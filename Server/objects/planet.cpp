#include <iostream>
#include <cmath>

#include "planet.h"
#include "objects/objects.h"
#include "objectmanager.h"

#define clean_angle(a) (((a+90)/180)*M_PI)

REGISTER_FACTORY(PLANET,Planet);

sf::Clock update_timer;

Planet::Planet(sf::String name, sf::Vector2f pos, sf::Vector2f vel, float rot, float rot_vel) :
Object(PLANET, name, pos, vel, rot, rot_vel) {
  angle = 0;
}

Planet::~Planet() {

}

void Planet::Update(float time) {
  angle += velocity.x * time;
  while( angle > 360 ) {
    angle -= 360;
  }
  while( angle < (-360) ) {
    angle += 360;
  }
  position.x = cos( clean_angle(angle) ) * velocity.y;
  position.y = sin( clean_angle(angle) ) * velocity.y;

  position += anchor;

  rotation += rotational_velocity * time;

  if( update_timer.GetElapsedTime() > 10 ) {
    SendUpdate();
    update_timer.Reset();
  }
}

void Planet::FillPartialPacket(sf::Packet& p) {
  Object::FillPartialPacket(p);
  p << angle << anchor.x << anchor.y;
}

void Planet::FillFullPacket(sf::Packet& p) {
  Object::FillFullPacket(p);
  p << angle << anchor.x << anchor.y;
}

void Planet::HandlePacket(sf::Packet& p) {

}
