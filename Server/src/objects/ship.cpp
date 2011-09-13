#include <iostream>

#include <SFML/Graphics.hpp>

#include "../networkmanager.h"
#include "../objectmanager.h"
#include "../utility.h"
#include "../player.h"
#include "../game.h"
#include "objects.h"
#include "ship.h"

#define clean_angle(a) (((a+90)/180)*M_PI)

REGISTER_FACTORY( SHIP,Ship );

Ship::Ship( sf::String name, sf::Vector2f position, sf::Vector2f velocity, float rotation, float rotational_velocity ) :
	Object( SHIP, name, position, velocity, rotation, rotational_velocity ),
  m_is_player( false ),
  m_acceleration( sf::Vector2f( 0, 0 ) ),
  m_thrust( 0 ) {
}

Ship::~Ship() {
}

void Ship::Update( float time ) {
  SetAcceleration( sf::Vector2f( -cos( clean_angle( GetRotation() ) ) * ( -GetThrust() ),
																	sin( clean_angle( GetRotation() ) ) * ( -GetThrust() ) ) );
  SetVelocity( GetVelocity() + ( GetAcceleration() * time ) );
  Object::Update( time );
}

void Ship::FillPartialPacket( PacketPtr packet ) {
  Object::FillPartialPacket( packet );
  (*packet) << GetThrust();
}

void Ship::FillFullPacket( PacketPtr packet ) {
  Object::FillFullPacket( packet );
  (*packet) << GetThrust();
}

void Ship::HandlePacket( PacketPtr packet ) {
  sf::Uint16 type1 = 1337;

  (*packet) >> type1;
  switch( type1 ) {
    case COMMAND_CONTROL: {
      sf::Uint16 code = 1337;
      (*packet) >> code;
      switch(code) {
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
          LogConsole( STRING_CAST( GetId() ) + " sent bogus code" );
          break;
      }
      break;
    }
    default:
      break;
  }

  SetDirty();
}

bool Ship::IsPlayer() {
	return m_is_player;
}

sf::Vector2f Ship::GetAcceleration() {
	return m_acceleration;
}

void Ship::SetAcceleration( sf::Vector2f acceleration ) {
	m_acceleration = acceleration;
}

float Ship::GetThrust() {
	return m_thrust;
}

void Ship::SetThrust( float thrust ) {
	m_thrust = thrust;
}
