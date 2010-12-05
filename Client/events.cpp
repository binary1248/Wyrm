#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "network.h"
#include "gui.h"
#include "events.h"

int HandleEvents(sf::RenderWindow& app) {
  sf::Event Event;
  while (app.GetEvent(Event))
  {
    if( !HandleGUIEvent( Event ) )
    {
      sf::Uint16 code = 1337;

      // Window closed
      if (Event.Type == sf::Event::Closed) {
        return 1;
      }

      // Escape key pressed
      if (Event.Type == sf::Event::KeyPressed) {
        switch(Event.Key.Code) {
          case sf::Key::Escape:
            return 1;
            break;
          case sf::Key::W:
            code = 0;
            break;
          case sf::Key::A:
            code = 2;
            break;
          case sf::Key::S:
            code = 4;
            break;
          case sf::Key::D:
            code = 6;
            break;
          default:
            break;
        }
      }
      if (Event.Type == sf::Event::KeyReleased) {
        switch(Event.Key.Code) {
          case sf::Key::W:
            code = 1;
            break;
          case sf::Key::A:
            code = 3;
            break;
          case sf::Key::S:
            code = 5;
            break;
          case sf::Key::D:
            code = 7;
            break;
          default:
            break;
        }
      }

      if( networkhandler.authenticated && code != 1337) {
        sf::Packet packet;
        packet << (sf::Uint16)COMMAND << (sf::Uint16)CONTROL << code;
        networkhandler.Send(packet);
      }
    }
  }

  return 0;
}
