#include <iostream>
#include <SFML/Network.hpp>

#include "game.h"
#include "networkmanager.h"

NetworkManager::NetworkManager() {
  listening = false;
  selector.Clear();

  if (sock_listener.Listen(1337) != sf::Socket::Done) {
    // Error...
    std::cout << "WTF? Why can't I listen???" << std::endl;
  }

  accept_selector.Add(sock_listener);
  listening = true;
}

NetworkManager::~NetworkManager() {
  accept_selector.Remove(sock_listener);
}

void NetworkManager::Tick(float time) {
  if( accept_selector.Wait(0.001) ) {
    //if(selector.IsReady(sock_listener)) {
      AcceptSocket();
    //}
  }
}

bool NetworkManager::IsListening() {
  return listening;
}

void NetworkManager::SelectorAdd(sf::TcpSocket* s) {
  selector.Add(*s);
}

void NetworkManager::SelectorRemove(sf::TcpSocket* s) {
  selector.Remove(*s);
}

bool NetworkManager::SelectorIsReady(sf::TcpSocket* s) {
  if( selector.Wait(0.001) ) {
    return selector.IsReady(*s);
  } else {
    return false;
  }
}

void NetworkManager::AcceptSocket() {
  sf::TcpSocket* Client = new sf::TcpSocket;

  if ( sock_listener.Accept(*Client) != sf::Socket::Done) {
    std::cout << "NetworkManager Accept error..." << std::endl;
    delete Client;
  } else {
    // Say hi
    sf::Packet packet;
    packet << sf::String("Wyrm protocol version ") << (float)PROTOCOL_VER_MAJOR << (float)PROTOCOL_VER_MINOR;
    sf::Socket::Status status = Client->Send(packet);

    if(status != sf::TcpSocket::Done) {
      std::cout << "Failed sending protocol version" << std::endl;
      std::cout.flush();
    }

    Game::getGame()->GetPlayerManager()->CreatePlayer(Client);
  }
}
