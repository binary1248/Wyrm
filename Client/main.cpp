#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "player.h"
#include "network.h"
#include "gui.h"
#include "game.h"
#include "objects/objects.h"
#include "objectmanager.h"

int main(int argc, char** argv) {

  Game::GetGame()->Run();

  delete Game::GetGame();

  return EXIT_SUCCESS;
}
