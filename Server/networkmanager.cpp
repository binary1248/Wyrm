#include <iostream>
#include <cstring>
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

  selector.Add(sock_listener);
  listening = true;
}

NetworkManager::~NetworkManager() {

}

void NetworkManager::Tick(float time) {
  if( selector.Wait(0.001) ) {
    if(selector.IsReady(sock_listener)) {
      AcceptSocket();
    }
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

void NetworkManager::AcceptSocket() {
  sf::TcpSocket* Client = new sf::TcpSocket;

  if ( sock_listener.Accept(*Client) != sf::Socket::Done) {
    std::cout << "NetworkManager Accept error..." << std::endl;
    delete Client;
  } else {
    Game::getGame()->GetPlayerManager()->CreatePlayer(Client);
  }
}
