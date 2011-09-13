#include <iostream>

#include <boost/make_shared.hpp>

#include "utility.h"
#include "game.h"

GamePtr Game::instance;

GamePtr Game::GetGame() {
  if( !Game::instance ) {
    instance.reset( new Game() );
  }

  return instance;
}

Game::Game() :
	m_running( false ),
	m_objectmanager( boost::make_shared<ObjectManager>() ),
	m_playermanager( boost::make_shared<PlayerManager>() ),
	m_networkmanager( boost::make_shared<NetworkManager>() ) {
}

Game::~Game() {
}

int Game::Run() {
  m_running = true;

  sf::Clock Clock;

  if( !m_networkmanager->IsListening() ) {
    return EXIT_FAILURE;
  }

  LogConsole( "Running..." );

  while ( m_running ) {
    float ElapsedTime = static_cast<float>( Clock.GetElapsedTime() ) / 1000.0f;
    Clock.Reset();

    Tick( ElapsedTime );

    int sleepTime = 10 - Clock.GetElapsedTime();
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

PlayerManagerPtr Game::GetPlayerManager() const {
	return m_playermanager;
}

ObjectManagerPtr Game::GetObjectManager() const {
	return m_objectmanager;
}

NetworkManagerPtr Game::GetNetworkManager() const {
	return m_networkmanager;
}
