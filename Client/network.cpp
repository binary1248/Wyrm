#include <iostream>
#include <SFML/Network.hpp>

#include "network.h"
#include "player.h"
#include "objectmanager.h"
#include "game.h"

NetworkHandler::NetworkHandler() {
  connected = false;
  authenticated = false;
}

NetworkHandler::~NetworkHandler() {
  Disconnect();
}

int NetworkHandler::Connect(sf::String username, sf::String password) {
  if (Client.Connect("localhost", 1337) != sf::Socket::Done)
  {
    // Error...
    std::cout << "Can't connect to server..." << std::endl;
    return -1;
  }

  connected = true;

  Selector.Add(Client);

  {
    sf::Packet packet;
    Client.Receive(packet);
    sf::String string;
    float version_major;
    float version_minor;
    packet >> string >> version_major >> version_minor;
    std::cout << string.ToAnsiString() << version_major << " " << version_minor << std::endl;
    if( !(string.ToAnsiString().compare("Wyrm protocol version ")) &&
         (version_major == PROTOCOL_VER_MAJOR) &&
         (version_minor == PROTOCOL_VER_MINOR) ) {
      sf::Packet packet;
      packet << username << password;
      Client.Send(packet);
    } else {
      std::cout << "Protocol mismatch" << std::endl;
      return -1;
    }
  }

  {
    sf::Packet packet;
    if( Client.Receive(packet) != sf::TcpSocket::Done )
      std::cout << "Error while receiving." << std::endl;
    sf::String s;
    packet >> s;
    if(!s.ToAnsiString().compare("Authentication successful")) {
      std::cout << "Authentication successful" << std::endl;
      authenticated = true;
    }
    else {
      std::cout << "Authentication failed: " << s.ToAnsiString() << std::endl;
      Disconnect();
      return -1;
    }
  }

  return 0;
}

void NetworkHandler::Disconnect() {
  Selector.Remove(Client);
  Client.Disconnect();
}

void NetworkHandler::Send(sf::Packet p) {
  Client.Send(p);
}

void NetworkHandler::HandlePacket(sf::Packet p) {

  sf::Uint16 type0;
  p >> type0;

  switch(type0) {
    case OBJECT:
    {
      Game::GetGame()->GetObjectManager()->DispatchPacket(p);
      break;
    }
    case SET_ID:
    {
      sf::Uint16 id;
      p >> id;
      Player* player = Game::GetGame()->GetPlayer();
      if(!player) {
        player = new Player(0,"");
      }
      player->SetShip(id);
      break;
    }
    default:
      break;
  }
}

void NetworkHandler::Tick() {
  if( !connected ) {
    return;
  }

  if( Selector.Wait(0.010) ) {
    sf::Packet packet;
    Client.Receive(packet);
    HandlePacket(packet);
  }
}

bool NetworkHandler::IsAuthenticated() {
  return (connected && authenticated);
}

bool NetworkHandler::IsConnected() {
  return connected;
}
