#include <config.hpp>
#include <utility.hpp>
#include <game.hpp>

GamePtr Game::m_instance;

GamePtr Game::GetGame() {
	if( !Game::m_instance ) {
		m_instance.reset( new Game() );
	}

	return m_instance;
}

Game::Game() :
	m_objectmanager( std::make_shared<ObjectManager>() ),
	m_playermanager( std::make_shared<PlayerManager>() ),
	m_networkmanager( std::make_shared<NetworkManager>() ),
	m_resourcemanager( std::make_shared<ResourceManager>() ),
	m_running( false ) {
	m_last_frame = std::chrono::high_resolution_clock::now();
}

Game::~Game() {
}

int Game::Run() {
	m_running = true;

	if( !m_networkmanager->IsListening() ) {
		return EXIT_FAILURE;
	}

	LogConsole( "Running..." );

	while ( m_running ) {
		auto current_frame = std::chrono::high_resolution_clock::now();
		auto micro_secs = std::chrono::duration_cast<std::chrono::microseconds>( current_frame - m_last_frame ).count();
		m_last_frame = current_frame;

		float ElapsedTime = static_cast<float>( micro_secs ) / 1000000.0f;

		Tick( ElapsedTime );

		current_frame = std::chrono::high_resolution_clock::now();
		auto milli_secs = std::chrono::duration_cast<std::chrono::milliseconds>( current_frame - m_last_frame ).count();
		int sleepTime = 10 - static_cast<int>( milli_secs );
		sleepTime = ( sleepTime > 0 ) ? sleepTime : 0;
		sf::sleep( sf::milliseconds( sleepTime ) ); // Limit 100 FPS
	}

	LogConsole( "Shutting down..." );;

	return EXIT_SUCCESS;
}

void Game::Stop() {
	m_running = false;
}

void Game::Tick( float time ) {
	m_networkmanager->Tick( time );
	m_resourcemanager->Tick( time );
	m_objectmanager->Tick( time );
	m_playermanager->Tick( time );
}

const PlayerManagerPtr& Game::GetPlayerManager() const {
	return m_playermanager;
}

const ObjectManagerPtr& Game::GetObjectManager() const {
	return m_objectmanager;
}

const NetworkManagerPtr& Game::GetNetworkManager() const {
	return m_networkmanager;
}

const ResourceManagerPtr& Game::GetResourceManager() const {
	return m_resourcemanager;
}
