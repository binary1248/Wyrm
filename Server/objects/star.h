#ifndef STAR_H_INCLUDED
#define STAR_H_INCLUDED

#include "object.h"

class Star : public Object {
  public:
    Star(sf::String name_ = "",
         sf::Vector2f pos_ = sf::Vector2f(0,0),
         sf::Vector2f vel_ = sf::Vector2f(0,0),
         float rot_ = 0, float rot_vel_ = 0);
    virtual ~Star();

    virtual void Update(float time);

    virtual void FillPartialPacket(sf::Packet& p);
    virtual void FillFullPacket(sf::Packet& p);
    virtual void HandlePacket(sf::Packet& p);

    inline void SetOrbit(float v, float r) { velocity.x = v; velocity.y = r; }
    inline void SetRotationalVelocity(float v) { rotational_velocity = v; }
    inline void SetAngle(float a) { angle = a; }
    inline void SetAnchor(float x, float y) { anchor.x = x; anchor.y = y; }

  private:
    float angle;
    sf::Vector2f anchor;

    sf::Clock update_timer;
};

#endif // STAR_H_INCLUDED
