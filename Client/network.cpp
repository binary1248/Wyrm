#include <iostream>
#include <SFML/Network.hpp>

#include "network.h"
#include "player.h"
#include "objectmanager.h"

NetworkHandler networkhandler;

NetworkHandler::NetworkHandler() {
  connected = false;
  running = false;
  authenticated = false;
}

NetworkHandler::~NetworkHandler() {

}

int NetworkHandler::Connect(sf::String username, sf::String password) {
  if (Client.Connect(/*"62.75.146.174"*/"localhost", 1337) != sf::Socket::Done)
  {
    // Error...
    std::cout << "Can't connect to server..." << std::endl;
    return -1;
  }

  connected = true;

  {
    sf::Packet packet;
    Client.Receive(packet);
    sf::String string;
    float version_major;
    float version_minor;
    packet >> string >> version_major >> version_minor;
    std::cout << string.ToAnsiString() << version_major << " " << version_minor << std::endl;
    //if( !(string.ToAnsiString().compare("Wyrm protocol version ")) &&
    //     (version_major == 0.1f) &&
    //     (version_minor == 0.1f) )
    {
      sf::Packet packet;
      packet << username << password;
      Client.Send(packet);
    }
    //else {
    //  std::cout << "Protocol mismatch" << std::endl;
    //  return -1;
    //}
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
      Client.Disconnect();
      return -1;
    }
  }

  if( connected && authenticated )
    Launch();

  return 0;
}

void NetworkHandler::Disconnect() {
  running = false;
}

void NetworkHandler::Send(sf::Packet p) {
  Client.Send(p);
}

void NetworkHandler::HandlePacket(sf::Packet p) {

  sf::Uint16 type0;
  p >> type0;

  switch(type0) {
    case OBJECT: {
      objectmanager.DispatchPacket(p);
      break;
    }
    default:
      break;
  }
}

void NetworkHandler::Run() {
  running = true;
  Selector.Add(Client);
  while(running) {
    if( Selector.Wait(0.010) ) {
      sf::Packet packet;
      Client.Receive(packet);
      HandlePacket(packet);
    }
  }
  Client.Disconnect();
}
