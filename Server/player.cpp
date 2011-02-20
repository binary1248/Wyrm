#include <iostream>
#include <SFML/Network.hpp>

#include "game.h"
#include "player.h"
#include "networkmanager.h"
#include "auth.h"

Player::Player(sf::TcpSocket* s, Object* o) {
  id = Game::GetGame()->GetPlayerManager()->NewID();
  name = s->GetRemoteAddress().ToString();
  connection = s;
  agent = 0;
  alive = true;
  half_open = true;
  Game::GetGame()->GetNetworkManager()->AddPlayer(this);
  std::cout << TIME << "Client " << GetIPAddressString() << " connected.." << std::endl;

  // Set agent
  SetAgent(o);

  // Subscribe all relevant objects
  Game::GetGame()->GetObjectManager()->SubscribeRelevant(this);
}

Player::~Player() {
  Game::GetGame()->GetNetworkManager()->RemovePlayer(this);

  std::cout << TIME << "Client " << (sf::Uint32)id << " " << GetIPAddressString() << " disconnected.." << std::endl;

  connection->Disconnect();
  delete connection;
  connection = 0;

  for( size_t i = 0; i < view.size(); i++ ) {
    view[i]->Unsubscribe(this);
  }

  Game::GetGame()->GetObjectManager()->RemoveObjectById( agent->GetId() );
}

void Player::Update() {
  FlushBuffer();
  if( !recv_buffer.empty() ) {
    Receive();
  }
}

void Player::Send( sf::Packet& p ) {
  sf::Socket::Status status = connection->Send(p);

  if(status != sf::TcpSocket::Done) {
    std::cout << TIME << "Failed sending data from client " << (sf::Uint32)id << ": " << ErrCode(status) << std::endl;
  }
}

void Player::FlushBuffer() {
  if( !alive || half_open ) {
    return;
  }

  while(!send_buffer.empty()) {
    Send(*(send_buffer.front()));
    delete (send_buffer.front());
    send_buffer.front() = 0;
    send_buffer.pop_front();
  }
}

void Player::SendPacket( sf::Packet& p, bool prio ) {
  if( !alive ) {
    return;
  }

  if( prio ) {
    Send(p);
  } else {
    sf::Packet* _p = new sf::Packet;
    _p->Append(p.GetData(), p.GetDataSize());
    send_buffer.push_back(_p);
  }
}

void Player::ReceivePacket(sf::Packet* p) {
  recv_buffer.push_back(p);
}

void Player::Receive() {
  sf::Packet p = *(recv_buffer.front());

  if(!half_open) {
    HandlePacket(p);
  } else {
    std::cout << TIME << "Client " << (sf::Uint32)id << " sent auth data" << std::endl;
    Auth(p);
  }

  delete recv_buffer.front();
  recv_buffer.pop_front();
}

void Player::HandlePacket(sf::Packet& p) {
  if( p.GetDataSize() < 4 ) {
    std::cout << TIME << "Player sent packet size < 4" << std::endl;
    return;
  }

  sf::Uint16 type0;
  p >> type0;

  switch( type0 ) {
    case COMMAND:
      agent->HandlePacket(p);
      break;
    default:
      std::cout << TIME << "Player sent packet with type0=" << type0 << std::endl;
      break;
  }
}

void Player::Auth(sf::Packet p) {
  if(CheckAuth(p)) {
    sf::Packet packet;
    packet << sf::String("Authentication successful");
    SendPacket(packet, true);

    std::cout << TIME << "Authentication successful" << std::endl;
    half_open = false;
  } else {
    sf::Packet packet;
    packet << sf::String("Authentication failed");
    SendPacket(packet, true);


    std::cout << TIME << "Authentication failed" << std::endl
              << "Client "
              << GetIPAddressString()
              << " disconnected (auth failure).."
              << std::endl;
    Kill();
  }
}

void Player::Kill() {
  if(alive) {
    alive = false;
  }
}

void Player::SetAgent(Object* o) {
  agent = o;
  o->SetName( sf::String(GetIPAddressString()) );
  AddObjectToView(o);
  sf::Packet packet;
  packet << (sf::Uint16)SET_ID << o->id;
  SendPacket(packet);
}

void Player::AddObjectToView(Object* o) {
  for( size_t i = 0; i < view.size(); i++ ) {
    if( view[i] == o ) {
      return;
    }
  }
  view.push_back(o);
  o->Subscribe(this);
}

void Player::RemoveObjectFromView(Object* o) {
  for( std::vector<Object*>::iterator i = view.begin(); i != view.end(); i++ ) {
    if( (*i) == o ) {
      view.erase(i);
      o->Unsubscribe(this);
      return;
    }
  }
}
