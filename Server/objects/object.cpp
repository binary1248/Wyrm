#include <iostream>

#include "object.h"
#include "networkmanager.h"
#include "player.h"
#include "game.h"

Object::Object(sf::Uint16 type, sf::String name, sf::Vector2f pos, sf::Vector2f vel, float rot, float rot_vel) {
  this->type = type;
  this->name = name;
  this->position = pos;
  this->velocity = vel;
  this->rotation = rot;
  this->rotational_velocity = rot_vel;

  removal = false;

  id = Game::GetGame()->GetObjectManager()->NewID();

  SetDirty();

  SetFresh();

  Game::GetGame()->GetObjectManager()->AddObject(this);

  std::cout << TIME << "Created object of type " << type << std::endl;
}

Object::~Object() {
  removal = true;
  for( std::set<Player*>::iterator i = subscribers.begin(); i != subscribers.end(); i++ ) {
    if( (*i)->Alive() ) {
      (*i)->RemoveObjectFromView(this);
    }
  }
}

void Object::Update(float time) {
  rotation += (rotational_velocity * time);
  position += (velocity * time);

  if( IsDirty() ) {
    SendUpdate();
  }
  ClearDirty();
}

void Object::FillPartialPacket(sf::Packet& p) {
  p << (sf::Uint16)OBJECT << id << (sf::Uint16)UPDATE
    << position.x << position.y << rotation
    << velocity.x << velocity.y << rotational_velocity;
}

void Object::FillFullPacket(sf::Packet& p) {
  p << (sf::Uint16)OBJECT << (sf::Uint16)id << (sf::Uint16)STATE
    << type       << name
    << position.x << position.y
    << velocity.x << velocity.y
    << rotation   << rotational_velocity;
}

void Object::SendUpdate() {
  sf::Packet p;
  FillPartialPacket(p);
  for( std::set<Player*>::iterator i = subscribers.begin(); i != subscribers.end(); i++ ) {
    (*i)->SendPacket(p);
  }
}

void Object::SendState() {
  sf::Packet p;
  FillFullPacket(p);
  for( std::set<Player*>::iterator i = subscribers.begin(); i != subscribers.end(); i++ ) {
    (*i)->SendPacket(p);
  }
}

void Object::Subscribe(Player* p) {
  if( removal ) {
    return;
  }

  subscribers.insert(p);
  sf::Packet packet;
  FillFullPacket(packet);
  p->SendPacket(packet);
}

void Object::Unsubscribe(Player* p) {
  if( !p->Alive() ) {
    return;
  }

  std::set<Player*>::iterator i = subscribers.find(p);
  if( i != subscribers.end() ) {
    sf::Packet packet;
    packet << (sf::Uint16)OBJECT << id << (sf::Uint16)REMOVE;
    (*i)->SendPacket(packet);
    if( !removal ) {
      subscribers.erase(i);
    }
  }
}
