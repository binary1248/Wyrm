#include "object.h"

Object::Object(sf::Uint16 type, sf::Uint16 id, sf::String name, sf::Vector2f pos, sf::Vector2f vel, float rot, float rot_vel) {
  this->type = type;
  this->id = id;
  this->name = name;
  this->position = pos;
  this->velocity = vel;
  this->rotation = rot;
  this->rotational_velocity = rot_vel;
}

Object::~Object() {

}

void Object::HandlePacket(sf::Packet& p) {

}

sf::Uint16 Object::GetId() {
  return id;
}

void Object::SetName( sf::String name ) {
  this->name = name;
}
