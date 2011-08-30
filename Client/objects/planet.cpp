#include <iostream>
#include <SFML/Graphics.hpp>

#include "game.h"
#include "network.h"
#include "objects/objects.h"
#include "objectmanager.h"
#include "planet.h"

#define clean_angle(a) (((a+90)/180)*M_PI)

REGISTER_FACTORY(PLANET,Planet);

Planet::Planet(sf::Uint16 id_, sf::String name_, sf::Vector2f pos_, sf::Vector2f vel_, float rot_, float rot_vel_) :
Object(PLANET, id_, name_, pos_, vel_, rot_, rot_vel_) {
  id = id_;
  name = name_;
  angle = 0;

  Init();
}

Planet::Planet(sf::Uint16 id_, sf::Packet& p) :
Object(PLANET, id_, "", sf::Vector2f(0,0), sf::Vector2f(0,0), 0, 0) {
  p >> name >> position.x >> position.y >> velocity.x >> velocity.y
    >> rotation >> rotational_velocity >> angle >> anchor.x >> anchor.y;

  Init();
}

Planet::~Planet() {
  if( image ) {
    delete image;
  }
}

void Planet::Init() {
  float amplitudes[] = {100,60,10,80,0,10,30,40};

  image = Game::GetGame()->GetResourceManager()->GetPlanet(100,
                                                           100,
                                                           sf::Color(0xDB,0x99,0x00,255),
                                                           4,
                                                           8,
                                                           (float*)amplitudes );

  Sprite.SetImage(*image);
  Sprite.SetOrigin(image->GetWidth()/2,image->GetHeight()/2);
  Sprite.SetScale(1.6,1.6);
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
    case OBJECT_UPDATE:
    {
      sf::Vector2f pos;
      sf::Vector2f vel;
      p >> pos.x >> pos.y >> rotation >> vel.x >> vel.y >> rotational_velocity >> angle;
      position = pos;
      velocity = vel;
      break;
    }
    case OBJECT_STATE:
    {
      sf::Uint16 type;
      sf::Vector2f pos;
      sf::Vector2f vel;
      p >> type >> name >> pos.x >> pos.y >> vel.x >> vel.y >> rotation >> rotational_velocity >> angle >> anchor.x >> anchor.y;
      position = pos;
      velocity = vel;
      break;
    }
    case OBJECT_REMOVE:
    {
      Delete();
      break;
    }
  }
}
