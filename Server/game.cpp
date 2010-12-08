#include <iostream>
#include "game.h"

Game::Game() {
  running = false;

  playermanager = new PlayerManager(this);
  objectmanager = new ObjectManager(this);
  networkmanager = new NetworkManager(this);
}

Game::~Game() {
  running = false;

  delete networkmanager;
  delete objectmanager;
  delete playermanager;
}

int Game::Run() {
  running = true;

  sf::Clock Clock;

  if( !networkmanager->IsListening() ) {
    return EXIT_FAILURE;
  }

  std::cout << "Running..." << std::endl;

  while (running) {
    float ElapsedTime = Clock.GetElapsedTime();
    Clock.Reset();

    Tick(ElapsedTime);

    sf::Sleep(0.02 - Clock.GetElapsedTime()); // Limit 50 FPS
  }

  std::cout << "Shutting down..." << std::endl;

  return EXIT_SUCCESS;
}

void Game::Tick(float time) {
  networkmanager->Tick(time);
  objectmanager->Tick(time);
  playermanager->Tick(time);
}

PlayerManager* Game::GetPlayerManager() {
  return playermanager;
}

ObjectManager* Game::GetObjectManager() {
  return objectmanager;
}

NetworkManager* Game::GetNetworkManager() {
  return networkmanager;
}
