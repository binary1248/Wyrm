#include <iostream>
#include <sstream>
#include "utility.h"
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

  delete_me = false;

  id = Game::GetGame()->GetObjectManager()->NewID();

  SetDirty();

  SetFresh();

  std::stringstream ss;
  ss << "Created object " << id << " of type " << type;
  LogConsole(ss.str());
}

Object::~Object() {
  std::stringstream ss;
  ss << "Destroyed object " << id << " of type " << type;
  LogConsole(ss.str());
}

void Object::Update(float time) {
  if( IsDeleted() ) {
    return;
  }

  rotation += (rotational_velocity * time);
  position += (velocity * time);
}

void Object::FillPartialPacket(sf::Packet& p) {
  p << (sf::Uint16)SERVER_OBJECT << id << (sf::Uint16)OBJECT_UPDATE
    << position.x << position.y << rotation
    << velocity.x << velocity.y << rotational_velocity;
}

void Object::FillFullPacket(sf::Packet& p) {
  p << (sf::Uint16)SERVER_OBJECT << (sf::Uint16)id << (sf::Uint16)OBJECT_STATE
    << type       << name
    << position.x << position.y
    << velocity.x << velocity.y
    << rotation   << rotational_velocity;
}
