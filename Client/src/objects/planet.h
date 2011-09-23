#ifndef PLANET_H_INCLUDED
#define PLANET_H_INCLUDED

#include "../resourcemanager.h"
#include "object.h"

class Planet : public Object{
  public:
    Planet(sf::Uint16 id_, sf::String name_, sf::Vector2f pos_, sf::Vector2f vel_, float rot_, float rot_vel_);
    Planet( sf::Uint16 id_, sf::Packet p );
    virtual ~Planet();

    virtual void Update(float time);
    virtual void Draw(sf::RenderWindow& w);

    virtual void HandlePacket(sf::Packet p);

  private:
    void Init();

    float angle;
    sf::Vector2f anchor;

    sf::Sprite Sprite;
    TexturePtr texture;
};

typedef boost::shared_ptr<Planet> PlanetPtr;
typedef boost::weak_ptr<Planet> PlanetWeakPtr;

#endif // PLANET_H_INCLUDED
