#include <iostream>
#include <SFML/Graphics.hpp>

#include "game.h"
#include "network.h"
#include "objects/objects.h"
#include "objectmanager.h"
#include "star.h"

#define clean_angle(a) (((a+90)/180)*M_PI)

REGISTER_FACTORY(STAR,Star);

Star::Star(sf::Uint16 id_, sf::String name_, sf::Vector2f pos_, sf::Vector2f vel_, float rot_, float rot_vel_) :
Object(STAR, id_, name_, pos_, vel_, rot_, rot_vel_) {
  id = id_;
  name = name_;
  angle = 0;
  sf::Image* image = Game::GetGame()->GetResourceManager()->OpenImage("planet.png");
  Sprite.SetImage(*image);
  Sprite.SetOrigin(image->GetWidth()/2,image->GetHeight()/2);
  Sprite.SetScale(0.4,0.4);
}

Star::Star(sf::Uint16 id_, sf::Packet& p) :
Object(STAR, id_, "", sf::Vector2f(0,0), sf::Vector2f(0,0), 0, 0) {
  p >> name >> position.x >> position.y >> velocity.x >> velocity.y
    >> rotation >> rotational_velocity >> angle >> anchor.x >> anchor.y;

  sf::Image* image = Game::GetGame()->GetResourceManager()->OpenImage("planet.png");
  Sprite.SetImage(*image);
  Sprite.SetOrigin(image->GetWidth()/2,image->GetHeight()/2);
  Sprite.SetScale(0.4,0.4);
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

  Sprite.SetPosition(position);
  Sprite.SetRotation(rotation);
}

void Star::Draw(sf::RenderWindow& w) {
  w.Draw(Sprite);
}

void Star::HandlePacket(sf::Packet p) {
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
      Delete();
      break;
    }
  }
}
