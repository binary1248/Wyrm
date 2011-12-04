#include <sstream>
#include <GL/gl.h>
#include <GL/glu.h>

#include <SFML/Window.hpp>

#include <backdrop.hpp>
#include <events.hpp>
#include <utility.hpp>
#include <game.hpp>

GamePtr Game::m_instance;

#define DEFAULT_WIDTH 1024
#define DEFAULT_HEIGHT 768

Game::Game() {
  // Create the main rendering window
  m_window = std::make_shared<sf::RenderWindow>( sf::VideoMode(DEFAULT_WIDTH, DEFAULT_HEIGHT, 32),
	                                               "WYRM",
	                                               sf::Style::Titlebar | sf::Style::Close,
	                                               sf::ContextSettings(24,    // Depth buffer
	                                                                    8,    // Stencil buffer
	                                                                    8) ); // AA level

	//m_window->EnableVerticalSync( true );

  LoadKeymap();

  m_window->EnableKeyRepeat( true );

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
  static unsigned int counter = 0;
  static sf::Clock fps_timer;

  while ( m_running && m_window->IsOpened() ) {
    float elapsed_time = static_cast<float>( m_window->GetFrameTime() ) / 1000.0f;

    Tick( elapsed_time );

    counter++;

    if( fps_timer.GetElapsedTime() > 1000 ) {
      fps_timer.Reset();
      m_window->SetTitle( "WYRM - " + string_cast( counter ) + " FPS" );
      counter = 0;
    }

    m_window->SetFramerateLimit(200);
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

const BackdropPtr& Game::GetBackdrop() const {
  return m_backdrop;
}

void Game::Resize( float width, float height ) {
  sf::View view( sf::FloatRect( 0, 0, width, height ) );
  m_window->SetView( view );
}

const sf::Vector2f Game::GetDefaultResolution() {
  return sf::Vector2f( DEFAULT_WIDTH, DEFAULT_HEIGHT );
}

const sf::Vector2f Game::GetCurrentResolution() {
  sf::View view = m_window->GetView();
  return view.GetSize();
}

void Game::Tick( float time ) {
  if( HandleEvents( *m_window ) ) {
    m_running = false;
  }

  m_networkhandler->Tick();

  // Clear the screen (fill it with black color)
  m_window->Clear( sf::Color( 0, 0, 0, 255 ) );

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  if( m_networkhandler->IsAuthenticated() ) {
    if( !m_backdrop ) {
      m_backdrop = std::make_shared<Backdrop>( m_window );
    }

    m_objectmanager->Tick( time );

    if( m_player ) {
      m_player->Tick( time );
    }

    m_backdrop->Draw( *m_window, time );

    m_objectmanager->DrawAll( *m_window );
  }

  m_gui->Draw( *m_window );

  // Display window contents on screen
  m_window->Display();
}
