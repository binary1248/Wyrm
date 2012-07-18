#include <SFML/Graphics.hpp>

#include <config.hpp>
#include <utility.hpp>
#include <networkmanager.hpp>
#include <objectmanager.hpp>
#include <utility.hpp>
#include <player.hpp>
#include <game.hpp>
#include <objects/objects.hpp>
#include <objects/ship.hpp>

REGISTER_FACTORY( ObjectType::SHIP, Ship );

Ship::Ship( sf::String name, const sf::Vector2f& size, const sf::Vector2f& position, const sf::Vector2f& velocity,
            float rotation, float rotational_velocity ) :
	Object( ObjectType::SHIP, name, size, position, velocity, rotation, rotational_velocity ),
	m_is_player( false ),
	m_acceleration( sf::Vector2f( 0, 0 ) ),
	m_thrust( 0 ) {
}

Ship::~Ship() {
}

void Ship::Update( float time ) {
	SetAcceleration( sf::Vector2f( static_cast<float>( -cos( clean_angle( GetRotation() ) ) ) * ( -GetThrust() ),
																 static_cast<float>(	sin( clean_angle( GetRotation() ) ) ) * ( -GetThrust() ) ) );

	SetVelocity( GetVelocity() + ( GetAcceleration() * time ) );

	Object::Update( time );
}

void Ship::FillPartialPacket( const PacketPtr& packet ) {
	Object::FillPartialPacket( packet );
	(*packet) << GetThrust();
}

void Ship::FillFullPacket( const PacketPtr& packet ) {
	Object::FillFullPacket( packet );
	(*packet) << GetThrust();
}

void Ship::HandlePacket( const PacketPtr& packet ) {
	sf::Uint16 type1 = 1337;

	( *packet ) >> type1;

	switch( type1 ) {
		case ClientToServerCommand::COMMAND_CONTROL: {
			sf::Uint16 code = 1337;
			(*packet) >> code;

			switch( code ) {
				case 0:
					SetThrust( 100 );
					break;
				case 1:
					SetThrust( 0 );
					break;
				case 2:
					SetRotationalVelocity( 200 );
					break;
				case 3:
					SetRotationalVelocity( 0 );
					break;
				case 4:
					break;
				case 5:
					break;
				case 6:
					SetRotationalVelocity( -200 );
					break;
				case 7:
					SetRotationalVelocity( 0 );
					break;
				default:
					LogConsole( string_cast( GetId() ) + " sent bogus type1" );
					break;
			}

			break;
		}
		default:
			break;
	}

	SetDirty();
}

bool Ship::IsPlayer() const {
	return m_is_player;
}

const sf::Vector2f& Ship::GetAcceleration() const {
	return m_acceleration;
}

void Ship::SetAcceleration( const sf::Vector2f& acceleration ) {
	m_acceleration = acceleration;
}

float Ship::GetThrust() const {
	return m_thrust;
}

void Ship::SetThrust( float thrust ) {
	m_thrust = thrust;
}
