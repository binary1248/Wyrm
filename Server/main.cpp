#include <iostream>
#include <vector>
#include <csignal>
#include <SFML/Network.hpp>

#include "playermanager.h"
#include "objectmanager.h"
#include "player.h"
#include "network.h"
#include "globals.h"

bool bRunning = true;

void terminate (int param)
{
  std::cout << "Terminating server..." << std::endl;
  bRunning = false;
}

int main(int argc, char** argv) {
  //signal (SIGTERM,terminate);
  bRunning = true;
  sf::Clock Clock;
  sf::Clock LastFullUpdate;
  if( listener.running ) {
    std::cout << "Running..." << std::endl;
    while (bRunning) {
      float ElapsedTime = Clock.GetElapsedTime();
      Clock.Reset();
      objectmanager.Tick(ElapsedTime);
      if( LastFullUpdate.GetElapsedTime() < 2.0f ) {
        objectmanager.SendPartialUpdate();
      }
      else {
        objectmanager.SendFullUpdate();
        LastFullUpdate.Reset();
      }
      sf::Sleep(0.02 - Clock.GetElapsedTime()); // Limit 50 FPS
    }
    std::cout << "Shutting down..." << std::endl;
    listener.running = false;
  }
  else {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
