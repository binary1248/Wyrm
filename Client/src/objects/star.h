#ifndef STAR_H_INCLUDED
#define STAR_H_INCLUDED

#include "object.h"
#include "../particlesystem/particlesystem.h"

class Star : public Object{
  public:
    Star(sf::Uint16 id_, sf::String name_, sf::Vector2f pos_, sf::Vector2f vel_, float rot_, float rot_vel_);
    Star( sf::Uint16 id_, sf::Packet p );
    virtual ~Star();

    virtual void Update(float time);
    virtual void Draw(sf::RenderWindow& w);

    virtual void HandlePacket(sf::Packet p);
  private:
    void CreateParticleSystem();
    float angle;
    sf::Vector2f anchor;

    ParticleSystemPtr ps;
};

typedef boost::shared_ptr<Star> StarPtr;
typedef boost::weak_ptr<Star> StarWeakPtr;

#endif // STAR_H_INCLUDED