#include <iostream>
#include "game.h"

Game::Game() {
  running = false;

  playermanager = new PlayerManager();
  objectmanager = new ObjectManager(playermanager);
  networkmanager = new NetworkManager(playermanager);
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
  sf::Clock LastFullUpdate;
  if( networkmanager->running ) {
    std::cout << "Running..." << std::endl;
    while (running) {
      float ElapsedTime = Clock.GetElapsedTime();
      Clock.Reset();
      objectmanager->Tick(ElapsedTime);
      if( LastFullUpdate.GetElapsedTime() < 2.0f ) {
        objectmanager->SendPartialUpdate();
      }
      else {
        objectmanager->SendFullUpdate();
        LastFullUpdate.Reset();
      }
      sf::Sleep(0.02 - Clock.GetElapsedTime()); // Limit 50 FPS
    }
    std::cout << "Shutting down..." << std::endl;
    networkmanager->running = false;
  }
  else {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
