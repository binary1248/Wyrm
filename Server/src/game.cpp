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
	m_running( false ) {
}

Game::~Game() {
}

int Game::Run() {
  m_running = true;

  sf::Clock clock;

  if( !m_networkmanager->IsListening() ) {
    return EXIT_FAILURE;
  }

  LogConsole( "Running..." );

  while ( m_running ) {
    float ElapsedTime = static_cast<float>( clock.GetElapsedTime() ) / 1000.0f;
    clock.Reset();

    Tick( ElapsedTime );

    int sleepTime = 10 - clock.GetElapsedTime();
    sleepTime = ( sleepTime > 0 ) ? sleepTime : 0;
    sf::Sleep( sleepTime ); // Limit 100 FPS
  }

  LogConsole( "Shutting down..." );;

  return EXIT_SUCCESS;
}

void Game::Stop() {
	m_running = false;
}

void Game::Tick( float time ) {
  m_networkmanager->Tick( time );
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
