#ifndef PLANET_H_INCLUDED
#define PLANET_H_INCLUDED

#include <boost/shared_ptr.hpp>

#include "object.h"

class Planet : public Object {
  public:
    Planet( sf::String name = "",
						sf::Vector2f position = sf::Vector2f( 0, 0 ),
						sf::Vector2f velocity = sf::Vector2f( 0, 0 ),
						float rotation = 0, float rotational_velocity = 0 );
    virtual ~Planet();

    virtual void Update( float time );

    virtual void FillPartialPacket( PacketPtr packet );
    virtual void FillFullPacket( PacketPtr packet );
    virtual void HandlePacket( PacketPtr packet );

    void SetOrbit( float velocity, float radius );
    void SetAngle( float angle );
    void SetAnchor( float x, float y );

  private:
    float m_angle;
    sf::Vector2f m_anchor;

    sf::Clock m_update_timer;
};

typedef boost::shared_ptr<Planet> PlanetPtr;

#endif // PLANET_H_INCLUDED
