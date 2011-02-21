#include <SFML/Network.hpp>

#include "network.h"
#include "gui.h"
#include "game.h"
#include "events.h"

sf::Uint16 keymap[sf::Event::Count][sf::Key::Count];

int HandleEvents(sf::RenderWindow& app) {
  sf::Event Event;
  while (app.GetEvent(Event))
  {
    if( !Game::GetGame()->GetGUI()->HandleEvent( Event ) )
    {
      sf::Uint16 code = 1337;

      // Window closed
      if (Event.Type == sf::Event::Closed) {
        return 1;
      }

      if (Event.Type == sf::Event::KeyPressed || Event.Type == sf::Event::KeyReleased) {
        if( Event.Key.Code == sf::Key::Escape ) {
          // Escape key pressed
          return 1;
        } else {
          code = keymap[Event.Type][Event.Key.Code];
        }
      }

      if( Game::GetGame()->GetNetworkHandler()->IsAuthenticated() && code != 1337) {
        sf::Packet packet;
        packet << (sf::Uint16)COMMAND << (sf::Uint16)CONTROL << code;
        Game::GetGame()->GetNetworkHandler()->Send(packet);
      }
    }
  }

  return 0;
}

void LoadKeymap() {

  for( size_t i = 0; i < sf::Event::Count; i++ ) {
    for( size_t j = 0; j < sf::Key::Count; j++ ) {
      keymap[i][j] = 1337;
    }
  }

  keymap[sf::Event::KeyPressed][sf::Key::W] = 0;
  keymap[sf::Event::KeyReleased][sf::Key::W] = 1;
  keymap[sf::Event::KeyPressed][sf::Key::A] = 2;
  keymap[sf::Event::KeyReleased][sf::Key::A] = 3;
  keymap[sf::Event::KeyPressed][sf::Key::S] = 4;
  keymap[sf::Event::KeyReleased][sf::Key::S] = 5;
  keymap[sf::Event::KeyPressed][sf::Key::D] = 6;
  keymap[sf::Event::KeyReleased][sf::Key::D] = 7;
}
