#include <GL/gl.h>
#include <GL/glu.h>
#include <SFML/Network.hpp>
#include <network.hpp>
#include <gui.hpp>
#include <game.hpp>
#include <events.hpp>

#define DOWN true
#define UP false

sf::Uint16 keymap[sf::Event::Count][sf::Keyboard::KeyCount];
bool keystate[sf::Keyboard::KeyCount];

int HandleEvents( sf::RenderWindow& window ) {
  sf::Event event;

  while( window.PollEvent( event ) ) {
    if( event.Type == sf::Event::Resized ) {
      Game::GetGame()->Resize( static_cast<float>( event.Size.Width ), static_cast<float>( event.Size.Height ) );
      glViewport( 0, 0, event.Size.Width, event.Size.Height );
    } else if( !Game::GetGame()->GetGUI()->HandleEvent( event ) ) {
      sf::Uint16 code = 1337;

      // Window closed
      if( event.Type == sf::Event::Closed ) {
        return 1;
      }

      if( ( event.Type == sf::Event::KeyPressed ) && ( keystate[event.Key.Code] != DOWN ) ) {
        //if( event.Key.Code == sf::Keyboard::Escape ) {
          // Escape key pressed
        //  return 1;
        //} else {
          code = keymap[event.Type][event.Key.Code];
        //}

        keystate[event.Key.Code] = DOWN;
      }

      if( ( event.Type == sf::Event::KeyReleased ) && ( keystate[event.Key.Code] != UP ) ) {
        code = keymap[event.Type][event.Key.Code];
        keystate[event.Key.Code] = UP;
      }

      if( Game::GetGame()->GetNetworkHandler()->IsAuthenticated() && ( code != 1337 ) ) {
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
