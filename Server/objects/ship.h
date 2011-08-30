#ifndef SHIP_H_INCLUDED
#define SHIP_H_INCLUDED

#include "object.h"

class Ship : public Object{
  public:
    Ship(sf::String name_ = "",
         sf::Vector2f pos_ = sf::Vector2f(0,0),
         sf::Vector2f vel_ = sf::Vector2f(0,0),
         float rot_ = 0, float rot_vel_ = 0);
    virtual ~Ship();

    virtual void Update(float time);

    virtual void FillPartialPacket(sf::Packet& p);
    virtual void FillFullPacket(sf::Packet& p);
    virtual void HandlePacket(sf::Packet& p);

    bool isPlayer;

    sf::Vector2f acceleration;
    float thrust;
};

#endif // SHIP_H_INCLUDED
