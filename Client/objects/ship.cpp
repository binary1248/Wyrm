#include <iostream>
#include <SFML/Graphics.hpp>

#include "ship.h"
#include "objects/objects.h"
#include "game.h"
#include "objectmanager.h"

#define clean_angle(a) (((a+90)/180)*M_PI)

REGISTER_FACTORY(SHIP,Ship);

Ship::Ship(sf::Uint16 id_, sf::String name_, sf::Vector2f pos_, sf::Vector2f vel_, float rot_, float rot_vel_) :
Object(SHIP, id_, name_, pos_, vel_, rot_, rot_vel_) {
  id = id_;
  name = name_;
  Init();
}

Ship::Ship(sf::Uint16 id_, sf::Packet& p) :
Object(SHIP, id_, "", sf::Vector2f(0,0), sf::Vector2f(0,0), 0, 0) {
  p >> name >> position.x >> position.y >> velocity.x >> velocity.y
    >> rotation >> rotational_velocity >> thrust;

  Init();
}

Ship::~Ship() {

}

void Ship::Init() {
  isPlayer = false;
  acceleration.x = 0;
  acceleration.y = 0;
  sf::Image* image = Game::GetGame()->GetResourceManager()->OpenImage("spaceship.png");
  Sprite.SetImage(*image);
  Sprite.SetOrigin(image->GetWidth()/2,image->GetHeight()/2);
  Sprite.SetScale(1,1);
  Text.SetFont(sf::Font::GetDefaultFont());
  Text.SetColor(sf::Color(0, 255, 0));
  Text.SetScale(0.4f, 0.4f);

  image_height = image->GetHeight();
}

void Ship::Update(float time) {
  rotation += (rotational_velocity * time);
  acceleration.y = sin( clean_angle(rotation) ) * (-thrust);
  acceleration.x = (-cos( clean_angle(rotation) )) * (-thrust);
  velocity += (acceleration * time);
  position += (velocity * time);
  Sprite.SetPosition(position);
  Sprite.SetRotation(-rotation);

  sf::FloatRect rect = Text.GetRect();
  Text.SetPosition(position + sf::Vector2f(-rect.Width/2,image_height/3));

  if( isPlayer ) {
    Game::GetGame()->GetBackdrop()->Update(velocity);
  }
}

void Ship::Draw(sf::RenderWindow& w) {
  if( isPlayer ) {
    sf::View View2(sf::FloatRect(0, 0, w.GetWidth(), w.GetHeight()));
    View2.SetCenter(position);
    w.SetView(View2);
  }
  Text.SetString(name);
  w.Draw(Sprite);
  w.Draw(Text);
}

void Ship::HandlePacket(sf::Packet p) {
  sf::Uint16 type1;
  p >> type1;

  switch(type1) {
    case UPDATE:
    {
      sf::Vector2f pos;
      sf::Vector2f vel;
      p >> pos.x >> pos.y >> rotation >> vel.x >> vel.y >> rotational_velocity >> thrust;
      position = pos;
      velocity = vel;
      break;
    }
    case STATE:
    {
      sf::Uint16 type;
      sf::Vector2f pos;
      sf::Vector2f vel;
      p >> type >> name >> pos.x >> pos.y >> vel.x >> vel.y >> rotation >> rotational_velocity >> thrust;
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
