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

	while( window.pollEvent( event ) ) {
		if( event.type == sf::Event::Resized ) {
			Game::GetGame()->Resize( static_cast<float>( event.size.width ), static_cast<float>( event.size.height ) );
			glViewport( 0, 0, event.size.width, event.size.height );
		} else if( !Game::GetGame()->GetGUI()->HandleEvent( event ) ) {
			sf::Uint16 code = 1337;

			// Window closed
			if( event.type == sf::Event::Closed ) {
				return 1;
			}

			if( ( event.type == sf::Event::KeyPressed ) && ( keystate[event.key.code] != DOWN ) ) {
				//if( event.key.code == sf::Keyboard::Escape ) {
					// Escape key pressed
				//	return 1;
				//} else {
					code = keymap[event.type][event.key.code];
				//}

				keystate[event.key.code] = DOWN;
			}

			if( ( event.type == sf::Event::KeyReleased ) && ( keystate[event.key.code] != UP ) ) {
				code = keymap[event.type][event.key.code];
				keystate[event.key.code] = UP;
			}

			if( Game::GetGame()->GetNetworkHandler()->IsAuthenticated() && ( code != 1337 ) ) {
				sf::Packet packet;

				packet << (sf::Uint16)ClientToServer::CLIENT_COMMAND << (sf::Uint16)ClientToServerCommand::COMMAND_CONTROL << code;

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
