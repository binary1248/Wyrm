#include <iostream>
#include <SFML/Graphics.hpp>

#include "objects/objects.h"
#include "network.h"
#include "ship.h"

#define clean_angle(a) (((a+90)/180)*M_PI)

Ship::Ship(sf::Uint16 id_, sf::String name_, sf::Vector2f pos_, sf::Vector2f vel_, float rot_, float rot_vel_) :
Object(SHIP, id_, name_, pos_, vel_, rot_, rot_vel_) {
  id = id_;
  name = name_;
  isPlayer = false;
  position.x = 0;
  position.y = 0;
  rotation = 0;
  velocity.x = 0;
  velocity.y = 0;
  rotational_velocity = 0;
  acceleration.x = 0;
  acceleration.y = 0;
  thrust = 0;
}

Ship::~Ship() {

}

void Ship::Update(float time) {
  rotation += (rotational_velocity * time);
  acceleration.y = sin( clean_angle(rotation) ) * (-thrust);
  acceleration.x = (-cos( clean_angle(rotation) )) * (-thrust);
  velocity += (acceleration * time);
  position += (velocity * time);
}

void Ship::HandlePacket(sf::Packet p) {
  sf::Uint16 type1 = 1337;

  p >> type1;
  switch(type1) {
    case CONTROL: {
      sf::Uint16 code = 1337;
      p >> code;
      switch(code) {
        case 0:
          //std::cout << id << " 0" << std::endl;
          thrust = 100;
          break;
        case 1:
          //std::cout << id << " 1" << std::endl;
          thrust = 0;
          break;
        case 2:
          //std::cout << id << " 2" << std::endl;
          //velocity.x = -100;
          rotational_velocity = 100;
          break;
        case 3:
          //std::cout << id << " 3" << std::endl;
          //velocity.x = 000;
          rotational_velocity = 000;
          break;
        case 4:
          //std::cout << id << " 4" << std::endl;
          //acceleration.y = sin( clean_angle(rotation) ) * (-100);
          //acceleration.x = cos( clean_angle(rotation) ) * (-100);
          break;
        case 5:
          //std::cout << id << " 5" << std::endl;
          //acceleration.y = 000;
          break;
        case 6:
          //std::cout << id << " 6" << std::endl;
          //velocity.x = 100;
          rotational_velocity = -100;
          break;
        case 7:
          //std::cout << id << " 7" << std::endl;
          //velocity.x = 000;
          rotational_velocity = 000;
          break;
        default:
          std::cout << id << " sent bogus code" << std::endl;
          break;
      }
      break;
    }
    default:
      break;
  }
}
