#include <iostream>
#include <SFML/Network.hpp>

#include "auth.h"
#include "player.h"
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
      return "NotReady";
    default:
      return "Undefined";
  }
}

NetworkManager::NetworkManager(PlayerManager* pm) {
  selector.Clear();
  //running = false;

  playermanager = pm;

  if (sock_listener.Listen(1337) != sf::Socket::Done)
  {
    // Error...
    std::cout << "WTF? Why can't I listen???" << std::endl;
  }
  running = true;
  selector.Add(sock_listener);
  Launch();
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
}*/

void NetworkManager::Run() {
  while( running ) {
    if( selector.Wait() ) {
      if(selector.IsReady(sock_listener)) {
        AcceptSocket();
      }
      else {
        HandleSockets();
      }
    }
    else {
      std::cout << "No sockets ready... this shouldn't happen..." << std::endl;
    }
  }
}

void NetworkManager::HandlePacket(sf::Packet p, sf::Uint16 id) {
  sf::Uint16 type0;
  p >> type0;

  if( type0 < 1 )
    playermanager->DispatchPacket(p, id);
}

void NetworkManager::AcceptSocket() {
  sf::TcpSocket* Client = new sf::TcpSocket;

  if (sock_listener.Accept(*Client) != sf::Socket::Done) {
    std::cout << "NetworkManager error..." << std::endl;
    delete Client;
  }
  else {
    sf::IpAddress ClientAddress = Client->GetRemoteAddress();
    std::cout << "Client " << ClientAddress.ToString() << " connected.." << std::endl;

    // Say hi
    sf::Packet packet;
    sf::String string("Wyrm protocol version ");
    packet << string;
    packet << (float)0.1f;
    packet << (float)0.1f;
    Client->Send(packet);

    sock_id_pair pair;
    pair.half_open = true;
    pair.s = Client;
    pair.id = 0xFFFF;

    clients.push_back(pair);
    selector.Add(*Client);
  }
}

void NetworkManager::HandleSockets() {
  for (iter = clients.begin(); iter != clients.end();) {
    if(!((*iter).s)) {
      std::cerr << "Invalid socket..." << std::endl;
      exit(-1);
    }
    sf::TcpSocket& client = *((*iter).s);
    if (selector.IsReady(client))
    {
      // The client has sent some data, we can receive it
      sf::Packet packet;
      sf::Socket::Status status = client.Receive(packet);
      if ( status == sf::Socket::Disconnected) {
        std::cout << "Client " << client.GetRemoteAddress().ToString() << " disconnected.." << std::endl;
        selector.Remove(client);
        playermanager->RemovePlayer((*iter).id);
        iter = clients.erase(iter);
        continue;
      }
      else if ( status == sf::Socket::NotReady) {
        std::cout << "Socket not ready..." << std::endl;
      }
      else if ( status == sf::Socket::Error ) {
        std::cout << "Socket error..." << std::endl;
      }
      else {
        if((*iter).half_open) {
          std::cout << "Checking auth" << std::endl;
          if(CheckAuth(packet)) {
            sf::Packet packet;
            sf::String s("Authentication successful");
            packet << s;
            sf::Socket::Status status = (*iter).s->Send(packet);
            if(status != sf::TcpSocket::Done)
              std::cerr << "Failed sending data: " << ErrCode(status) << std::endl;
            std::cout << s.ToAnsiString() << std::endl;
            (*iter).id = playermanager->CreatePlayer((*iter).s->GetRemoteAddress().ToString(), (*iter).s);
            (*iter).half_open = false;
          }
          else {
            sf::Packet packet;
            sf::String s("Authentication failed");
            packet << s;
            sf::Socket::Status status = (*iter).s->Send(packet);
            if(status != sf::Socket::Done)
              std::cerr << "Failed sending data: " << ErrCode(status) << std::endl;
            (*iter).s->Disconnect();
            std::cout << s.ToAnsiString() << std::endl;
            std::cout << "Client " << client.GetRemoteAddress().ToString() << " disconnected (auth failure).." << std::endl;
            delete (*iter).s;
            (*iter).s = 0;
            selector.Remove(client);
            iter = clients.erase(iter);
            continue;
          }
        }
        else {
          HandlePacket(packet, (*iter).id);
        }
      }
    }
    iter++;
  }
}
