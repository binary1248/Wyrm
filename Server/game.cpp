#include <iostream>
#include <sstream>
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

  std::cout << TIME << "Running..." << std::endl;

  while (running) {
    float ElapsedTime = Clock.GetElapsedTime();
    Clock.Reset();

    Tick(ElapsedTime);

    sf::Sleep(0.02 - Clock.GetElapsedTime()); // Limit 50 FPS
  }

  std::cout << TIME << "Shutting down..." << std::endl;

  return EXIT_SUCCESS;
}

void Game::Tick(float time) {
  networkmanager->Tick(time);
  objectmanager->Tick(time);
  playermanager->Tick(time);
}

std::string Game::GetTime() {
  float time = clock.GetElapsedTime();
  std::stringstream ss;
  ss.setf( std::ios::fixed, std::ios::floatfield );
  ss.fill('0');
  ss.width(9);
  ss.precision(4);
  ss << time;
  return "[" + ss.str() + "] ";
}
