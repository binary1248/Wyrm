#ifndef PLANET_HPP_INCLUDED
#define PLANET_HPP_INCLUDED

#include <memory>

#include <objects/object.hpp>

class Planet : public Object {
  public:
    Planet( sf::String name = "",
						const sf::Vector2f& position = sf::Vector2f( 0, 0 ),
						const sf::Vector2f& velocity = sf::Vector2f( 0, 0 ),
						float rotation = 0, float rotational_velocity = 0 );
    virtual ~Planet();

    virtual void Update( float time );

    virtual void FillPartialPacket( const PacketPtr& packet );
    virtual void FillFullPacket( const PacketPtr& packet );
    virtual void HandlePacket( const PacketPtr& packet );

    void SetOrbit( float velocity, float radius );
    void SetAngle( float angle );
    void SetAnchor( float x, float y );

  private:
    float m_angle;
    sf::Vector2f m_anchor;

    sf::Clock m_update_timer;
};

typedef std::shared_ptr<Planet> PlanetPtr;

#endif // PLANET_HPP_INCLUDED
