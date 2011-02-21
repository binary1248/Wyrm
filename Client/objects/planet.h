#ifndef PLANET_H_INCLUDED
#define PLANET_H_INCLUDED

#include "object.h"

class Planet : public Object{
  public:
    Planet(sf::Uint16 id_, sf::String name_, sf::Vector2f pos_, sf::Vector2f vel_, float rot_, float rot_vel_);
    Planet(sf::Uint16 id_, sf::Packet& p);
    virtual ~Planet();

    virtual void Update(float time);
    virtual void Draw(sf::RenderWindow& w);

    virtual void HandlePacket(sf::Packet p);

    float angle;
    sf::Vector2f anchor;

    sf::Sprite Sprite;
    sf::Image Image;
};

#endif // PLANET_H_INCLUDED
