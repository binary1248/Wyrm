#include <iostream>
#include <SFML/Network.hpp>

#include "auth.h"
#include "player.h"
#include "game.h"
#include "networkmanager.h"

std::string ErrCode(sf::Socket::Status s) {
  switch(s) {
    case sf::TcpSocket::Disconnected:
      return "Disconnected";
    case sf::TcpSocket::Done:
      return "Done";
    case sf::TcpSocket::Error:
      return "Error";
    case sf::TcpSocket::NotReady:
      return "Not Ready";
    default:
      return "Undefined";
  }
}

ClientSocket::ClientSocket(sf::TcpSocket* s) {
  this->s = s;
  half_open = true;
  id = 0xFFFF;
}

ClientSocket::~ClientSocket() {

}

sf::TcpSocket* ClientSocket::GetSocket() {
  return s;
}

sf::Uint16 ClientSocket::GetId() {
  return id;
}

bool ClientSocket::IsHalfOpen() {
  return half_open;
}

void ClientSocket::SetId(sf::Uint16 id) {
  this->id = id;
}

void ClientSocket::SetHalfOpen(bool half_open) {
  this->half_open = half_open;
}

NetworkManager::NetworkManager(Game* g) {
  listening = false;
  selector.Clear();

  game = g;

  if (sock_listener.Listen(1337) != sf::Socket::Done)
  {
    // Error...
    std::cout << "WTF? Why can't I listen???" << std::endl;
  }

  selector.Add(sock_listener);
  listening = true;
}

NetworkManager::~NetworkManager() {

}

/*
void NetworkManager::RemoveClient(sf::Uint16 id) {
  std::cout << "Removed client with id " << id << " from clients list." << std::endl;
  for( iter = clients.begin(); iter != clients.end(); iter++) {
    if(id == (*iter).id) {
      std::cout << "Removed client with id " << (*iter).id << " from clients list." << std::endl;
      iter = clients.erase(iter);
      return;
    }
  }
  std::cout << "This is not good." << std::endl;
}
*/

void NetworkManager::Tick(float time) {
  if( selector.Wait(0.001) ) {
    if(selector.IsReady(sock_listener)) {
      AcceptSocket();
    }
    HandleSockets();
  }
}

bool NetworkManager::IsListening() {
  return listening;
}

void NetworkManager::HandlePacket(sf::Packet p, sf::Uint16 id) {
  sf::Uint16 type0;
  p >> type0;

  if( type0 < 1 )
    game->GetPlayerManager()->DispatchPacket(p, id);
}

void NetworkManager::AcceptSocket() {
  sf::TcpSocket* Client = new sf::TcpSocket;

  if ( sock_listener.Accept(*Client) != sf::Socket::Done) {
    std::cout << "NetworkManager Accept error..." << std::endl;
    delete Client;
  }
  else {
    ClientConnect(Client);
  }
}

void NetworkManager::HandleSockets() {
  for (iter = clients.begin(); iter != clients.end(); iter++) {
    ClientSocket* pair = (*iter);

    if(!(pair->GetSocket())) {
      std::cerr << "Invalid socket..." << std::endl;
      exit(-1);
    }

    sf::TcpSocket& client = *(pair->GetSocket());

    if (selector.IsReady(client)) {
      // The client has sent some data, we can receive it
      sf::Packet packet;
      sf::Socket::Status status = client.Receive(packet);

      switch( status ) {
        case sf::Socket::Disconnected:
          ClientDisconnect(client, pair);
          continue;
          break;

        case sf::Socket::Done:
          if(pair->IsHalfOpen()) {
            if( !ClientAuth(client, pair, packet) ) {
              continue;
            }
          } else {
            HandlePacket(packet, pair->GetId());
          }
          break;

        default:
          std::cout << "Socket " << ErrCode(status) << "..." << std::endl;
          break;
      }
    }
  }
}

void NetworkManager::ClientDisconnect(sf::TcpSocket& client, ClientSocket* pair) {
  std::cout << "Client " << client.GetRemoteAddress().ToString() << " disconnected.." << std::endl;
  selector.Remove(client);
  game->GetPlayerManager()->RemovePlayer(pair->GetId());
  iter = clients.erase(iter);
}

void NetworkManager::ClientConnect(sf::TcpSocket* Client) {
  sf::IpAddress ClientAddress = Client->GetRemoteAddress();
  std::cout << "Client " << ClientAddress.ToString() << " connected.." << std::endl;

  // Say hi
  sf::Packet packet;
  packet << sf::String("Wyrm protocol version ") << (float)PROTOCOL_VER_MAJOR << (float)PROTOCOL_VER_MINOR;
  Client->Send(packet);

  ClientSocket* pair = new ClientSocket(Client);
  clients.push_back(pair);
  selector.Add(*Client);
}

bool NetworkManager::ClientAuth(sf::TcpSocket& client, ClientSocket* pair, sf::Packet& packet) {
  std::cout << "Checking auth" << std::endl;

  if(CheckAuth(packet)) {
    sf::Packet packet;
    packet << sf::String("Authentication successful");
    sf::Socket::Status status = pair->GetSocket()->Send(packet);

    if(status != sf::TcpSocket::Done) {
      std::cerr << "Failed sending data: " << ErrCode(status) << std::endl;
    }

    std::cout << "Authentication successful" << std::endl;
    pair->SetId( game->GetPlayerManager()->CreatePlayer(pair->GetSocket()->GetRemoteAddress().ToString(), pair->GetSocket()) );
    pair->SetHalfOpen(false);
    return true;
  } else {
    sf::Packet packet;
    packet << sf::String("Authentication failed");
    sf::Socket::Status status = pair->GetSocket()->Send(packet);

    if(status != sf::Socket::Done) {
      std::cerr << "Failed sending data: " << ErrCode(status) << std::endl;
    }

    pair->GetSocket()->Disconnect();
    std::cout << "Authentication failed" << std::endl
              << "Client "
              << client.GetRemoteAddress().ToString()
              << " disconnected (auth failure).."
              << std::endl;
    delete pair->GetSocket();
    selector.Remove(client);
    delete pair;
    pair = 0;
    iter = clients.erase(iter);
    return false;
  }
}
