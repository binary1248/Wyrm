#include <GL/gl.h>
#include <GL/glu.h>
#include <SFML/Network.hpp>
#include "network.h"
#include "gui.h"
#include "game.h"
#include "events.h"

#define DOWN true
#define UP false

sf::Uint16 keymap[sf::Event::Count][sf::Keyboard::KeyCount];
bool keystate[sf::Keyboard::KeyCount];

int HandleEvents(sf::RenderWindow& app) {
  sf::Event Event;
  while( app.PollEvent(Event) )
  {
    if( Event.Type == sf::Event::Resized ) {
      Game::GetGame()->Resize( Event.Size.Width, Event.Size.Height );
      glViewport(0, 0, Event.Size.Width, Event.Size.Height);
    } else if( !Game::GetGame()->GetGUI()->HandleEvent( Event ) ) {
      sf::Uint16 code = 1337;

      // Window closed
      if( Event.Type == sf::Event::Closed ) {
        return 1;
      }

      if( (Event.Type == sf::Event::KeyPressed) && (keystate[Event.Key.Code] != DOWN) ) {
        if( Event.Key.Code == sf::Keyboard::Escape ) {
          // Escape key pressed
          return 1;
        } else {
          code = keymap[Event.Type][Event.Key.Code];
        }
        keystate[Event.Key.Code] = DOWN;
      }

      if( (Event.Type == sf::Event::KeyReleased) && (keystate[Event.Key.Code] != UP) ) {
        code = keymap[Event.Type][Event.Key.Code];
        keystate[Event.Key.Code] = UP;
      }

      if( Game::GetGame()->GetNetworkHandler()->IsAuthenticated() && code != 1337) {
        sf::Packet packet;
        packet << (sf::Uint16)CLIENT_COMMAND << (sf::Uint16)COMMAND_CONTROL << code;
        Game::GetGame()->GetNetworkHandler()->Send(packet);
      }
    }
  }

  return 0;
}

void LoadKeymap() {

  for( size_t i = 0; i < sf::Event::Count; i++ ) {
    for( size_t j = 0; j < sf::Keyboard::KeyCount; j++ ) {
      keymap[i][j] = 1337;
    }
  }

  for( size_t i = 0; i < sf::Keyboard::KeyCount; i++ ) {
    keystate[i] = UP;
  }

  keymap[sf::Event::KeyPressed][sf::Keyboard::W] = 0;
  keymap[sf::Event::KeyReleased][sf::Keyboard::W] = 1;
  keymap[sf::Event::KeyPressed][sf::Keyboard::A] = 2;
  keymap[sf::Event::KeyReleased][sf::Keyboard::A] = 3;
  keymap[sf::Event::KeyPressed][sf::Keyboard::S] = 4;
  keymap[sf::Event::KeyReleased][sf::Keyboard::S] = 5;
  keymap[sf::Event::KeyPressed][sf::Keyboard::D] = 6;
  keymap[sf::Event::KeyReleased][sf::Keyboard::D] = 7;
}
