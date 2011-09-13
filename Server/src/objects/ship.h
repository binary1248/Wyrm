#ifndef SHIP_H_INCLUDED
#define SHIP_H_INCLUDED

#include <boost/shared_ptr.hpp>

#include "object.h"

class Ship : public Object{
  public:
    Ship( sf::String name = "",
					sf::Vector2f position = sf::Vector2f( 0, 0 ),
					sf::Vector2f velocity = sf::Vector2f( 0, 0 ),
					float rotation = 0, float rotational_velocity = 0 );
    virtual ~Ship();

    virtual void Update( float time );

    virtual void FillPartialPacket( PacketPtr packet );
    virtual void FillFullPacket( PacketPtr packet );
    virtual void HandlePacket( PacketPtr packet );

		bool IsPlayer();

		sf::Vector2f GetAcceleration();
		void SetAcceleration( sf::Vector2f acceleration );

		float GetThrust();
		void SetThrust( float thrust );

	private:
    bool m_is_player;

    sf::Vector2f m_acceleration;
    float m_thrust;
};

typedef boost::shared_ptr<Ship> ShipPtr;

#endif // SHIP_H_INCLUDED
