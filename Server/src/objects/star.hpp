#ifndef STAR_HPP_INCLUDED
#define STAR_HPP_INCLUDED

#include <memory>

#include <objects/object.hpp>

class Star : public Object {
  public:
    Star( sf::String name = "", const sf::Vector2f& size = sf::Vector2f( 1.0f, 1.0f ),
					const sf::Vector2f& position = sf::Vector2f( 0, 0 ),
					const sf::Vector2f& velocity = sf::Vector2f( 0, 0 ),
					float rotation = 0, float rotational_velocity = 0 );
    virtual ~Star();

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

typedef std::shared_ptr<Star> StarPtr;

#endif // STAR_HPP_INCLUDED
