#include <iostream>
#include <cmath>

#include "star.h"
#include "objects/objects.h"
#include "objectmanager.h"

#define clean_angle(a) (((a+90)/180)*M_PI)

REGISTER_FACTORY(STAR,Star);

Star::Star(sf::String name, sf::Vector2f pos, sf::Vector2f vel, float rot, float rot_vel) :
Object(STAR, name, pos, vel, rot, rot_vel) {
  angle = 0;
}

Star::~Star() {

}

void Star::Update(float time) {
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
    SetDirty();
    update_timer.Reset();
  }
}

void Star::FillPartialPacket(sf::Packet& p) {
  Object::FillPartialPacket(p);
  p << angle << position.x << position.y << rotation;
}

void Star::FillFullPacket(sf::Packet& p) {
  Object::FillFullPacket(p);
  p << angle << position.x << position.y << rotation << anchor.x << anchor.y;
}

void Star::HandlePacket(sf::Packet& p) {

}
