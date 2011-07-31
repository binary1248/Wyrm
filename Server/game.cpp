#include <iostream>
#include <sstream>
#include "utility.h"
#include "game.h"

Game* Game::instance;

Game* Game::GetGame() {
  if( !Game::instance ) {
    instance = new Game();
  }
  return instance;
}

Game::Game() {
  running = false;

  objectmanager = new ObjectManager();
  playermanager = new PlayerManager();
  networkmanager = new NetworkManager();
}

Game::~Game() {
  running = false;

  delete networkmanager;
  delete playermanager;
  delete objectmanager;
}

int Game::Run() {
  running = true;

  sf::Clock Clock;

  if( !networkmanager->IsListening() ) {
    return EXIT_FAILURE;
  }

  LogConsole("Running...");

  while (running) {
    float ElapsedTime = (float)(Clock.GetElapsedTime())/1000.0f;
    Clock.Reset();

    Tick(ElapsedTime);

    int sleepTime = 2 - Clock.GetElapsedTime();

    sleepTime = ( sleepTime > 0 ) ? sleepTime : 0;

    //sf::Sleep( sleepTime ); // Limit 50 FPS
  }

  LogConsole("Shutting down...");;

  return EXIT_SUCCESS;
}

void Game::Tick(float time) {
  networkmanager->Tick(time);
  objectmanager->Tick(time);
  playermanager->Tick(time);
}


