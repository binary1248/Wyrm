#include <iostream>
#include <SFML/Network.hpp>

#include "game.h"
#include "networkmanager.h"

NetworkManager::NetworkManager() {
  listening = false;
  selector.Clear();

  if (sock_listener.Listen(1337) != sf::Socket::Done) {
    std::cout << TIME << "Failed to listen on port 1337" << std::endl;
  }

  selector.Add(sock_listener);
  listening = true;
}

NetworkManager::~NetworkManager() {
  selector.Clear();
}

void NetworkManager::Tick(float time) {
  if( selector.Wait(0.001) ) {
    if( selector.IsReady(sock_listener) ) {
      AcceptSocket();
    }
    for( std::set<Player*>::iterator i = players.begin(); i != players.end(); i++ ) {
      sf::TcpSocket* s = (*i)->GetConnection();
      if( selector.IsReady(*s) ) {
        sf::Packet* p = new sf::Packet;
        sf::Socket::Status status = s->Receive(*p);

        switch( status ) {
          case sf::Socket::Disconnected:
            std::cout << TIME << "Client " << (sf::Uint32)(*i)->GetId() << " connection reset" << std::endl;
            delete p;
            (*i)->Kill();
            break;
          case sf::Socket::Done:
            (*i)->ReceivePacket(p);
            break;
          default:
            delete p;
            std::cout << TIME << "Player socket status: " << ErrCode(status) << "..." << std::endl;
            break;
        }
      }
    }
  }
}

bool NetworkManager::IsListening() {
  return listening;
}

void NetworkManager::AddPlayer(Player* p) {
  if( players.find(p) != players.end() ) {
    std::cout << TIME << "NetworkManager tried to insert existing player" << std::endl;
    return;
  }
  players.insert(p);
  selector.Add(*(p->GetConnection()));
}

void NetworkManager::RemovePlayer(Player* p) {
  if( players.find(p) == players.end() ) {
    std::cout << TIME << "NetworkManager tried to remove non-existing player" << std::endl;
    return;
  }
  selector.Remove(*(p->GetConnection()));
  players.erase(p);
}

void NetworkManager::AcceptSocket() {
  sf::TcpSocket* Client = new sf::TcpSocket;

  if ( sock_listener.Accept(*Client) != sf::Socket::Done ) {
    std::cout << TIME << "NetworkManager Accept error..." << std::endl;
    delete Client;
  } else {
    // Send protocol info
    sf::Packet packet;
    packet << sf::String("Wyrm protocol version ") << (float)PROTOCOL_VER_MAJOR << (float)PROTOCOL_VER_MINOR;
    sf::Socket::Status status = Client->Send(packet);

    if(status != sf::TcpSocket::Done) {
      std::cout << TIME << "Failed sending protocol version: " << ErrCode(status) << std::endl;
    }

    Game::GetGame()->GetPlayerManager()->CreatePlayer(Client);
  }
}
