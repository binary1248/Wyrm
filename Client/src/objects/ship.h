#ifndef SHIP_H_INCLUDED
#define SHIP_H_INCLUDED

#include "object.h"

class Ship : public Object{
  public:
    Ship(sf::Uint16 id_, sf::String name_, sf::Vector2f pos_, sf::Vector2f vel_, float rot_, float rot_vel_);
    Ship( sf::Uint16 id_, sf::Packet p );
    virtual ~Ship();

    virtual void Update(float time);
    virtual void Draw(sf::RenderWindow& w);

    virtual void HandlePacket(sf::Packet p);
    inline void SetPlayer(bool b) { isPlayer = b; }
    inline bool IsPlayer() { return isPlayer; }
  private:
    void Init();
    bool isPlayer;

    sf::Vector2f acceleration;
    float thrust;

    sf::Sprite Sprite;
    sf::Text Text;

    float image_height;
};

typedef boost::shared_ptr<Ship> ShipPtr;
typedef boost::weak_ptr<Ship> ShipWeakPtr;

#endif // SHIP_H_INCLUDED