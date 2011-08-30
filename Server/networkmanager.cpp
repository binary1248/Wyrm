#include <iostream>
#include <sstream>
#include <SFML/Network.hpp>

#include "game.h"
#include "player.h"
#include "utility.h"
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

NetworkManager::NetworkManager() {
  listening = false;
  selector.Clear();

  if (sock_listener.Listen(1337) != sf::Socket::Done) {
    LogConsole("Failed to listen on port 1337");
  }

  selector.Add(sock_listener);
  listening = true;
}

NetworkManager::~NetworkManager() {
  selector.Clear();
}

void NetworkManager::Tick(float time) {
  if( selector.Wait(1) ) {
    if( selector.IsReady(sock_listener) ) {
      AcceptSocket();
    }
    for( std::map<Player*, sf::TcpSocket*>::iterator i = players.begin(); i != players.end(); i++ ) {
      sf::TcpSocket* s = i->second;
      Player* player = i->first;
      if( selector.IsReady(*s) ) {
        ReceiveData(player, s);
      }
    }
  }
}

void NetworkManager::ReceiveData(Player* player, sf::TcpSocket* s) {
  sf::Packet* p = new sf::Packet;
  sf::Socket::Status status = s->Receive(*p);

  std::stringstream ss;

  switch( status ) {
    case sf::Socket::Disconnected:
      ss << "Client " << (sf::Uint32)(player->GetId()) << " connection reset";
      LogConsole(ss.str());
      delete p;
      player->Delete();
      break;
    case sf::Socket::Done:
      player->ReceivePacket(p);
      break;
    default:
      delete p;
      ss << "Player socket status: " << ErrCode(status) << "...";
      LogConsole(ss.str());
      break;
  }
}

void NetworkManager::SendData(Player* player, sf::Packet& p) {
  sf::TcpSocket* s = players[player];

  sf::Socket::Status status = s->Send(p);

  if(status != sf::TcpSocket::Done) {
    std::stringstream ss;
    ss << "Failed sending data from client " << (sf::Uint32)(player->GetId()) << ": " << ErrCode(status);
    LogConsole(ss.str());
  }
}

bool NetworkManager::IsListening() {
  return listening;
}

void NetworkManager::AddPlayer(Player* p, sf::TcpSocket* s) {
  if( players.find(p) != players.end() ) {
    LogConsole("NetworkManager tried to insert existing player");
    return;
  }
  players.insert( std::make_pair<Player*, sf::TcpSocket*>(p,s) );
  selector.Add(*s);

  std::stringstream ss;
  ss << p->GetName() << " (" << s->GetRemoteAddress().ToString() << ") connected..";
  LogConsole(ss.str());
}

void NetworkManager::RemovePlayer(Player* p) {
  if( players.find(p) == players.end() ) {
    LogConsole("NetworkManager tried to remove non-existing player");
    return;
  }

  sf::TcpSocket* socket = players[p];

  std::stringstream ss;
  ss << p->GetName() << " (" << socket->GetRemoteAddress().ToString() << ") disconnected..";
  LogConsole(ss.str());

  selector.Remove(*socket);

  socket->Disconnect();
  delete socket;
  socket = 0;

  players.erase(p);
}

void NetworkManager::AcceptSocket() {
  sf::TcpSocket* Client = new sf::TcpSocket;

  if ( sock_listener.Accept(*Client) != sf::Socket::Done ) {
    LogConsole("NetworkManager Accept error...");
    delete Client;
  } else {
    // Send protocol info
    sf::Packet packet;
    packet << sf::String("Wyrm protocol version ") << (float)PROTOCOL_VER_MAJOR << (float)PROTOCOL_VER_MINOR;
    sf::Socket::Status status = Client->Send(packet);

    if(status != sf::TcpSocket::Done) {
      std::stringstream ss;
      ss << "Failed sending protocol version: " << ErrCode(status);
      LogConsole(ss.str());
    }

    Player* p = Game::GetGame()->GetPlayerManager()->CreatePlayer();

    if( p ) {
      AddPlayer(p, Client);
    } else {
      Client->Disconnect();
      delete Client;
    }
  }
}
