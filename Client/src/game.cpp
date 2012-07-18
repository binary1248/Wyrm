#include <sstream>
#include <GL/gl.h>
#include <GL/glu.h>

#include <SFML/Window.hpp>

#include <system.hpp>
#include <events.hpp>
#include <utility.hpp>
#include <game.hpp>

GamePtr Game::m_instance;

#define DEFAULT_WIDTH 1024
#define DEFAULT_HEIGHT 768

Game::Game() {
	// Create the main rendering window
	m_window = std::make_shared<sf::RenderWindow>( sf::VideoMode( DEFAULT_WIDTH, DEFAULT_HEIGHT, 32 ),
	                                               "WYRM",
	                                               sf::Style::Titlebar | sf::Style::Close,
	                                               sf::ContextSettings( 0, // Depth buffer
	                                               0, // Stencil buffer
	                                               0, // AA level
	                                               2, // OpenGL Major Version
	                                               1 ) );// OpenGL Minor Version

	glShadeModel( GL_SMOOTH );

	glDisable( GL_DEPTH_TEST );

	m_window->setVerticalSyncEnabled( false );

	LoadKeymap();

	m_window->setKeyRepeatEnabled( true );

	m_gui = std::make_shared<GUI>( m_window );

	m_resourcemanager = std::make_shared<ResourceManager>();
	m_objectmanager = std::make_shared<ObjectManager>();
	m_networkhandler = std::make_shared<NetworkHandler>();

	m_running = true;
}

Game::~Game() {
}

const GamePtr& Game::GetGame() {
	if( !m_instance ) {
		m_instance.reset( new Game() );
	}

	return m_instance;
}

void Game::Run() {
	m_frame_clock.restart();

	long counter = 0;
	float fps_micro_secs = 0.0f;

	while ( m_running && m_window->isOpen() ) {
		float micro_secs = static_cast<float>( m_frame_clock.getElapsedTime().asMicroseconds() );
		m_frame_clock.restart();

		Tick( micro_secs / 1000000.0f );

		counter++;

		fps_micro_secs += micro_secs;

		if( fps_micro_secs > 1000000.0f ) {
			fps_micro_secs = 0.0f;
			m_window->setTitle( "WYRM - " + string_cast( counter ) + " FPS" );
			counter = 0;
		}
	}
}

void Game::Stop() {
	m_running = false;
}

const GUIPtr& Game::GetGUI() const {
	return m_gui;
}

const PlayerPtr& Game::GetPlayer() const {
	return m_player;
}

const PlayerPtr& Game::CreatePlayer( sf::Uint16 id, std::string name ) {
	if( !m_player ) {
		m_player = std::make_shared<Player>( id , name );
	}

	return m_player;
}

const NetworkHandlerPtr& Game::GetNetworkHandler() const {
	return m_networkhandler;
}

const ObjectManagerPtr& Game::GetObjectManager() const {
	return m_objectmanager;
}

const ResourceManagerPtr& Game::GetResourceManager() const {
	return m_resourcemanager;
}

const SystemPtr& Game::GetSystem() const {
	return m_system;
}

void Game::SetSystem( const SystemPtr& system ) {
	m_system = system;
}

void Game::Resize( float width, float height ) {
	sf::View view( sf::FloatRect( 0, 0, width, height ) );
	m_window->setView( view );
}

const sf::Vector2f Game::GetDefaultResolution() const {
	return sf::Vector2f( DEFAULT_WIDTH, DEFAULT_HEIGHT );
}

const sf::Vector2f Game::GetCurrentResolution() const {
	sf::View view = m_window->getView();
	return view.getSize();
}

const sf::View& Game::GetCurrentView() const {
	return m_window->getView();
}

const std::shared_ptr<sf::RenderWindow>& Game::GetWindow() const {
	return m_window;
}

void Game::Tick( float time ) {
	if( HandleEvents( *m_window ) ) {
		m_running = false;
	}

	m_networkhandler->Tick();

	m_window->clear( sf::Color( 0, 0, 0, 255 ) );

	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();

	glOrtho( 0.0f, m_window->getSize().x, m_window->getSize().y, 0.0f, -512.0f, 512.0f );

	PlayerPtr player = GetPlayer();

	if( player && player->GetAgent() ) {
		glTranslatef( static_cast<float>( m_window->getSize().x ) / 2.0f - player->GetAgent()->GetPosition().x,
									static_cast<float>( m_window->getSize().y ) / 2.0f - player->GetAgent()->GetPosition().y,
									0.0f );
	}

	glMatrixMode( GL_MODELVIEW );

	if( m_networkhandler->IsAuthenticated() ) {
		m_objectmanager->Tick( time );

		if( m_player ) {
			m_player->Tick( time );
		}

		if( m_system ) {
			m_system->Draw( *m_window, time );
		} else {
			m_window->clear( sf::Color( 0, 0, 0, 255 ) );
		}

		m_objectmanager->DrawAll( *m_window );
	} else {
		m_window->clear( sf::Color( 0, 0, 0, 255 ) );
	}

	m_window->pushGLStates();

	m_gui->Draw( *m_window );

	m_window->popGLStates();

	glMatrixMode( GL_PROJECTION );
	glPopMatrix();

	// Display window contents on screen
	m_window->display();
}
