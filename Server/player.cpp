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

  // Say hi
  sf::Packet packet;
  packet << sf::String("Wyrm protocol version ") << (float)PROTOCOL_VER_MAJOR << (float)PROTOCOL_VER_MINOR;
  SendPacket(packet);
}

Player::~Player() {
  Disconnect();
}

void Player::SelectSocket(sf::SocketSelector s) {
  if( s.IsReady(*connection) ) {
    ReceivePacket();
  }
}

void Player::SendPacket(sf::Packet p) {
  sf::Socket::Status status = connection->Send(p);

  if(status != sf::TcpSocket::Done) {
    std::cerr << "Failed sending data: " << ErrCode(status) << std::endl;
  }
}

void Player::ReceivePacket() {
  sf::Packet p;
  sf::Socket::Status status = connection->Receive(p);

  switch( status ) {
    case sf::Socket::Disconnected:
      Disconnect();
      break;
    case sf::Socket::Done:
      if(!half_open) {
        HandlePacket(p);
      } else {
        CheckAuth(p);
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

  if( payload ) {
    delete[] payload;
  }
}

void Player::Auth(sf::Packet p) {
  std::cout << "Checking auth" << std::endl;

  if(CheckAuth(p)) {
    sf::Packet packet;
    packet << sf::String("Authentication successful");
    SendPacket(packet);

    std::cout << "Authentication successful" << std::endl;
    half_open = false;
  } else {
    sf::Packet packet;
    packet << sf::String("Authentication failed");
    SendPacket(packet);


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
    std::cout << "Client " << GetIPAddressString() << " disconnected.." << std::endl;
    connection->Disconnect();
    //listener.RemoveClient(id);
    delete connection;
    connection = 0;
    connected = false;
    Game::getGame()->GetNetworkManager()->SelectorRemove(connection);
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
}
