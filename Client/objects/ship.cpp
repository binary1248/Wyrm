#include <iostream>
#include <SFML/Graphics.hpp>

#include "objects/objects.h"
#include "network.h"
#include "backdrop.h"
#include "ship.h"

#define clean_angle(a) (((a+90)/180)*M_PI)

sf::Image Image;

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
  Sprite.SetImage(Image);
  Sprite.SetOrigin(Image.GetWidth()/2,Image.GetHeight()/2);
  Text.SetString(name);
  Text.SetFont(sf::Font::GetDefaultFont());
  Text.SetScale(sf::Vector2f(2.0f,2.0f));
  Text.SetColor(sf::Color(255, 255, 255));
  Text.SetScale(0.4f, 0.4f);
}

Ship::~Ship() {

}

void Ship::Update(float time) {
  rotation += (rotational_velocity * time);
  acceleration.y = sin( clean_angle(rotation) ) * (-thrust);
  acceleration.x = (-cos( clean_angle(rotation) )) * (-thrust);
  velocity += (acceleration * time);
  position += (velocity * time);
  Sprite.SetPosition(position);
  Sprite.SetRotation(rotation);
  Text.SetPosition(position + sf::Vector2f(-33,22));
  if( isPlayer ) {
    UpdateBackdrop(velocity);
  }
}

void Ship::Draw(sf::RenderWindow& w) {
  if( isPlayer ) {
    sf::View View2(sf::FloatRect(0, 0, w.GetWidth(), w.GetHeight()));
    View2.SetCenter(position);
    w.SetView(View2);
  }
  w.Draw(Sprite);
  w.Draw(Text);
}

void Ship::HandlePacket(sf::Packet p) {
  sf::Uint16 type1;
  sf::Vector2f val;
  float val2;

  p >> type1 >> val.x >> val.y >> val2;

  switch(type1) {
    case VELOCITY_UPDATE:
      velocity = val;
      rotational_velocity = val2;
      p >> thrust;
      break;
    case POSITION_UPDATE:
      position = val;
      rotation = val2;

      break;
    default:
      break;
  }

}
