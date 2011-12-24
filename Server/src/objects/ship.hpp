#ifndef SHIP_HPP_INCLUDED
#define SHIP_HPP_INCLUDED

#include <memory>

#include <objects/object.hpp>

class Ship : public Object{
  public:
    Ship( sf::String name = "", const sf::Vector2f& size = sf::Vector2f( 1.0f, 1.0f ),
					const sf::Vector2f& position = sf::Vector2f( 0, 0 ),
					const sf::Vector2f& velocity = sf::Vector2f( 0, 0 ),
					float rotation = 0, float rotational_velocity = 0 );
    virtual ~Ship();

    virtual void Update( float time );

    virtual void FillPartialPacket( const PacketPtr& packet );
    virtual void FillFullPacket( const PacketPtr& packet );
    virtual void HandlePacket( const PacketPtr& packet );

		bool IsPlayer() const;

		const sf::Vector2f& GetAcceleration() const;
		void SetAcceleration( const sf::Vector2f& acceleration );

		float GetThrust() const;
		void SetThrust( float thrust );

	private:
    bool m_is_player;

    sf::Vector2f m_acceleration;
    float m_thrust;
};

typedef std::shared_ptr<Ship> ShipPtr;

#endif // SHIP_HPP_INCLUDED
