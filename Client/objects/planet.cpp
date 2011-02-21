#include <iostream>
#include <SFML/Graphics.hpp>

#include "planet.h"
#include "network.h"
#include "objects/objects.h"
#include "objectmanager.h"

#define clean_angle(a) (((a+90)/180)*M_PI)

REGISTER_FACTORY(PLANET,Planet);

Planet::Planet(sf::Uint16 id_, sf::String name_, sf::Vector2f pos_, sf::Vector2f vel_, float rot_, float rot_vel_) :
Object(PLANET, id_, name_, pos_, vel_, rot_, rot_vel_) {
  id = id_;
  name = name_;
  angle = 0;
  Image.LoadFromFile("planet.png");
  Sprite.SetImage(Image);
  Sprite.SetOrigin(Image.GetWidth()/2,Image.GetHeight()/2);
  Sprite.SetScale(0.4,0.4);
}

Planet::Planet(sf::Uint16 id_, sf::Packet& p) :
Object(PLANET, id_, "", sf::Vector2f(0,0), sf::Vector2f(0,0), 0, 0) {
  p >> name >> position.x >> position.y >> velocity.x >> velocity.y
    >> rotation >> rotational_velocity >> angle >> anchor.x >> anchor.y;

  Image.LoadFromFile("planet.png");
  Sprite.SetImage(Image);
  Sprite.SetOrigin(Image.GetWidth()/2,Image.GetHeight()/2);
  Sprite.SetScale(0.4,0.4);
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

  Sprite.SetPosition(position);
  Sprite.SetRotation(rotation);
}

void Planet::Draw(sf::RenderWindow& w) {
  w.Draw(Sprite);
}

void Planet::HandlePacket(sf::Packet p) {
  sf::Uint16 type1;
  p >> type1;

  switch(type1) {
    case UPDATE:
    {
      sf::Vector2f pos;
      sf::Vector2f vel;
      p >> pos.x >> pos.y >> rotation >> vel.x >> vel.y >> rotational_velocity >> angle >> anchor.x >> anchor.y;
      position = pos;
      velocity = vel;
      break;
    }
    case STATE:
    {
      sf::Uint16 type;
      sf::Vector2f pos;
      sf::Vector2f vel;
      p >> type >> name >> pos.x >> pos.y >> vel.x >> vel.y >> rotation >> rotational_velocity >> angle >> anchor.x >> anchor.y;
      position = pos;
      velocity = vel;
      break;
    }
    case REMOVE:
    {
      SetForRemoval();
      break;
    }
  }
}
