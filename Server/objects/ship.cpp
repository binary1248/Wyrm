#include <iostream>
#include <SFML/Graphics.hpp>

#include "ship.h"
#include "networkmanager.h"
#include "objects/objects.h"
#include "objectmanager.h"
#include "player.h"
#include "game.h"

#define clean_angle(a) (((a+90)/180)*M_PI)

REGISTER_FACTORY(SHIP,Ship);

Ship::Ship(sf::String name, sf::Vector2f pos, sf::Vector2f vel, float rot, float rot_vel) :
Object(SHIP, name, pos, vel, rot, rot_vel) {
  isPlayer = false;
  acceleration.x = 0;
  acceleration.y = 0;
  thrust = 0;
}

Ship::~Ship() {

}

void Ship::Update(float time) {
  acceleration.y = sin( clean_angle(rotation) ) * (-thrust);
  acceleration.x = (-cos( clean_angle(rotation) )) * (-thrust);
  velocity += (acceleration * time);
  Object::Update(time);
}

void Ship::FillPartialPacket(sf::Packet& p) {
  Object::FillPartialPacket(p);
  p << thrust;
}

void Ship::FillFullPacket(sf::Packet& p) {
  Object::FillFullPacket(p);
  p << thrust;
}

void Ship::HandlePacket(sf::Packet& p) {
  sf::Uint16 type1 = 1337;

  p >> type1;
  switch(type1) {
    case CONTROL: {
      sf::Uint16 code = 1337;
      p >> code;
      switch(code) {
        case 0:
          //std::cout << TIME << id << " 0" << std::endl;
          thrust = 100;
          break;
        case 1:
          //std::cout << TIME << id << " 1" << std::endl;
          thrust = 0;
          break;
        case 2:
          //std::cout << TIME << id << " 2" << std::endl;
          //velocity.x = -100;
          rotational_velocity = 100;
          break;
        case 3:
          //std::cout << TIME << id << " 3" << std::endl;
          //velocity.x = 000;
          rotational_velocity = 000;
          break;
        case 4:
          //std::cout << TIME << id << " 4" << std::endl;
          //acceleration.y = sin( clean_angle(rotation) ) * (-100);
          //acceleration.x = cos( clean_angle(rotation) ) * (-100);
          break;
        case 5:
          //std::cout << TIME << id << " 5" << std::endl;
          //acceleration.y = 000;
          break;
        case 6:
          //std::cout << TIME << id << " 6" << std::endl;
          //velocity.x = 100;
          rotational_velocity = -100;
          break;
        case 7:
          //std::cout << TIME << id << " 7" << std::endl;
          //velocity.x = 000;
          rotational_velocity = 000;
          break;
        default:
          std::cout << TIME << id << " sent bogus code" << std::endl;
          break;
      }
      break;
    }
    default:
      break;
  }

  SetDirty();
}
