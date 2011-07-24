#include <iostream>
#include <sstream>
#include "game.h"
#include "player.h"
#include "utility.h"
#include "networkmanager.h"
#include "auth.h"

Player::Player(Object* o) {
  id = Game::GetGame()->GetPlayerManager()->NewID();
  agent = 0;
  delete_me = false;
  half_open = true;

  std::stringstream ss;
  ss << "Player " << id;
  name = ss.str();

  // Subscribe all relevant objects
  Game::GetGame()->GetObjectManager()->SubscribeRelevant(this);

  // Set agent
  SetAgent(o);
}

Player::~Player() {
  for( size_t i = 0; i < view.size(); i++ ) {
    view[i]->Unsubscribe(this);
  }

  Game::GetGame()->GetNetworkManager()->RemovePlayer(this);

  agent->Delete();

  while( send_buffer.size() ) {
    if( send_buffer.front() ) {
      delete send_buffer.front();
    }
    send_buffer.pop_front();
  }

  while( recv_buffer.size() ) {
    if( recv_buffer.front() ) {
      delete recv_buffer.front();
    }
    recv_buffer.pop_front();
  }
}

void Player::Update() {
  FlushBuffer();
  HandleSocketData();
}

void Player::Send( sf::Packet& p ) {
  Game::GetGame()->GetNetworkManager()->SendData(this, p);
}

void Player::FlushBuffer() {
  if( IsDeleted() || half_open ) {
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
  if( IsDeleted() ) {
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

void Player::HandleSocketData() {
  if( recv_buffer.empty() ) {
    return;
  }

  sf::Packet p = *(recv_buffer.front());

  if(!half_open) {
    HandlePacket(p);
  } else {
    std::stringstream ss;
    ss << "Client " << (sf::Uint32)id << " sent auth data";
    LogConsole(ss.str());

    Auth(p);
  }

  delete recv_buffer.front();
  recv_buffer.pop_front();
}

void Player::HandlePacket(sf::Packet& p) {
  if( p.GetDataSize() < 4 ) {
    LogConsole("Player sent packet size < 4");
    return;
  }

  sf::Uint16 type0;
  p >> type0;

  switch( type0 ) {
    case COMMAND:
      agent->HandlePacket(p);
      break;
    default:
      std::stringstream ss;
      ss << "Player sent packet with type0=" << type0;
      LogConsole(ss.str());

      break;
  }
}

void Player::Auth(sf::Packet p) {
  if(CheckAuth(p)) {
    sf::Packet packet;
    packet << sf::String("Authentication successful");
    SendPacket(packet, true);

    LogConsole("Authentication successful");
    half_open = false;
  } else {
    sf::Packet packet;
    packet << sf::String("Authentication failed");
    SendPacket(packet, true);

    std::stringstream ss;
    ss << "Authentication failed: "
       << GetName()
       << " disconnected (auth failure)..";
    LogConsole(ss.str());

    Delete();
  }
}

void Player::SetAgent(Object* o) {
  agent = o;
  o->SetName( GetName() );
  sf::Packet packet;
  packet << (sf::Uint16)SET_ID << o->GetId();
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
