#include <iostream>
#include <cmath>
#include <cstring>
#include <SFML/Network.hpp>

#include "game.h"
#include "player.h"
#include "networkmanager.h"
#include "auth.h"

#define clean_angle(a) (((a+90)/180)*M_PI)

Player::Player(sf::Uint16 id_, sf::TcpSocket* s) {
  id = id_;
  name = s->GetRemoteAddress().ToString();
  connection = s;
  agent = 0;
  connected = true;
  half_open = true;
  Game::getGame()->GetNetworkManager()->SelectorAdd(connection);
  std::cout << "Client " << GetIPAddressString() << " connected.." << std::endl;
}

Player::~Player() {
  std::cout << "Destroying Player "  << (sf::Uint32)id << std::endl;
  Disconnect();
}

void Player::Update() {
  if( Game::getGame()->GetNetworkManager()->SelectorIsReady(connection) ) {
    ReceivePacket();
  }
}

void Player::Send( sf::Packet p )
{
  sf::Socket::Status status = connection->Send(p);

  if(status != sf::TcpSocket::Done) {
    std::cout << "Failed sending data from client " << (sf::Uint32)id << ": " << ErrCode(status) << std::endl;
    std::cout.flush();
  }
}

void Player::SendPacket( sf::Packet p, bool prio ) {
  if( !connection ) {
    return;
  }

  if( half_open && !prio ) {
    sf::Packet* _p = new sf::Packet;
    _p->Append(p.GetData(), p.GetDataSize());
    buffer.push_back(_p);

    return;
  } else if( prio ) {
    Send(p);
  } else {
    while(!buffer.empty()) {
      Send(*(buffer.front()));
      buffer.pop_front();
    }

    Send(p);
  }
}

void Player::ReceivePacket() {
  sf::Packet p;
  sf::Socket::Status status = connection->Receive(p);

  switch( status ) {
    case sf::Socket::Disconnected:
      std::cout << "Client " << (sf::Uint32)id << " connection reset" << std::endl;
      Disconnect();
      break;
    case sf::Socket::Done:
      if(!half_open) {
        //std::cout << "Client " << (sf::Uint32)id << " sent data" << std::endl;
        HandlePacket(p);
      } else {
        std::cout << "Client " << (sf::Uint32)id << " sent auth data" << std::endl;
        Auth(p);
      }
      break;
    default:
      std::cout << "Player socket status: " << ErrCode(status) << "..." << std::endl;
      break;
  }
}

void Player::HandlePacket(sf::Packet p) {
  char* payload = 0;
  std::size_t payload_size = p.GetDataSize();
  if( payload_size ) {
    payload = new char[payload_size];
    memcpy(payload, p.GetData(), payload_size);
  }

  if( payload_size < 4 ) {
    std::cout << "Packet size < 4" << std::endl;
    return;
  }

  sf::Uint16 type0;
  p >> type0;

  if( type0 < 1 ) {
    agent->HandlePacket(p);
  }

  /*
  for( size_t i = 0; i < payload_size; i++ ) {
    if( isprint(payload[i]) ) {
      std::cout << payload[i];
    } else {
      std::cout << ".";
    }
  }

  std::cout << std::endl;
  */

  if( payload ) {
    delete[] payload;
  }
}

void Player::Auth(sf::Packet p) {
  std::cout << "Checking auth" << std::endl;

  if(CheckAuth(p)) {
    sf::Packet packet;
    packet << sf::String("Authentication successful");
    std::cout.flush();
    SendPacket(packet, true);

    std::cout << "Authentication successful" << std::endl;
    half_open = false;
  } else {
    sf::Packet packet;
    packet << sf::String("Authentication failed");
    SendPacket(packet, true);


    std::cout << "Authentication failed" << std::endl
              << "Client "
              << GetIPAddressString()
              << " disconnected (auth failure).."
              << std::endl;
    Disconnect();
  }
}

void Player::Disconnect() {
  if(connection) {
    std::cout << "Disconnect Player " << (sf::Uint32)id << std::endl;
    std::cout << "Client " << GetIPAddressString() << " disconnected.." << std::endl;
    connection->Disconnect();
    Game::getGame()->GetNetworkManager()->SelectorRemove(connection);
    //listener.RemoveClient(id);
    delete connection;
    connection = 0;
    connected = false;
  }
}

bool Player::Connected() {
  return connected;
}

std::string Player::GetIPAddressString() {
  return connection->GetRemoteAddress().ToString();
}

sf::Uint16 Player::GetId() {
  return id;
}

Object* Player::GetAgent() {
  return agent;
}

void Player::SetAgent(Object* o) {
  agent = o;
  o->SetName( sf::String(GetIPAddressString()) );
  //std::cout << "Set player name to " << GetIPAddressString() << std::endl;
}
