#include <iostream>

#include "object.h"

Object::Object(sf::Uint16 type_, sf::Uint16 id_, sf::String name_, sf::Vector2f pos_, sf::Vector2f vel_, float rot_, float rot_vel_) :
type(type_), id(id_), name(name_), position(pos_), velocity(vel_), rotation(rot_), rotational_velocity(rot_vel_) {
  delete_me = false;
}

Object::~Object() {

}

void Object::Draw(sf::RenderWindow& w) {}

void Object::HandlePacket(sf::Packet p) {
  std::cout << "Object::HandlePacket was called" << std::endl;
}
