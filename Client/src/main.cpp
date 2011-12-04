#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <config.hpp>
#include <player.hpp>
#include <network.hpp>
#include <gui.hpp>
#include <game.hpp>
#include <objects/objects.hpp>
#include <objectmanager.hpp>

int main( int /*argc*/, char** /*argv*/ ) {
  Game::GetGame()->Run();

  return EXIT_SUCCESS;
}
