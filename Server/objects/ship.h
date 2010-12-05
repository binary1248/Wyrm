#ifndef SHIP_H_INCLUDED
#define SHIP_H_INCLUDED

#include "object.h"

class Ship : public Object{
  public:
    Ship(sf::Uint16 id_, sf::String name_, sf::Vector2f pos_, sf::Vector2f vel_, float rot_, float rot_vel_);
    virtual ~Ship();

    virtual void Update(float time);

    virtual void HandlePacket(sf::Packet p);

    bool isPlayer;

    sf::Vector2f acceleration;
    float thrust;
};

#endif // SHIP_H_INCLUDED
