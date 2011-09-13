#include <iostream>
#include <cmath>

#include "../objectmanager.h"
#include "objects.h"
#include "star.h"

#define clean_angle(a) (((a+90)/180)*M_PI)

REGISTER_FACTORY( STAR, Star );

Star::Star( sf::String name, sf::Vector2f position, sf::Vector2f velocity, float rotation, float rotational_velocity ) :
	Object( STAR, name, position, velocity, rotation, rotational_velocity ),
  m_angle( 0 )  {
}

Star::~Star() {
}

void Star::Update( float time ) {
  m_angle += GetVelocity().x * time;

  while( m_angle > 360 ) {
    m_angle -= 360;
  }
  while( m_angle < (-360) ) {
    m_angle += 360;
  }

  SetPosition( sf::Vector2f( cos( clean_angle(m_angle) ) * GetVelocity().y,
														 sin( clean_angle(m_angle) ) * GetVelocity().y ) );

  SetPosition( GetPosition() + m_anchor );

  SetRotation( GetRotation() + GetRotationalVelocity() * time );

  if( m_update_timer.GetElapsedTime() > 10000 ) {
    SetDirty();
    m_update_timer.Reset();
  }
}

void Star::FillPartialPacket( PacketPtr packet ) {
  Object::FillPartialPacket( packet );
  (*packet) << m_angle << GetPosition().x << GetPosition().y << GetRotation();
}

void Star::FillFullPacket( PacketPtr packet ) {
  Object::FillFullPacket( packet );
  (*packet) << m_angle << GetPosition().x << GetPosition().y << GetRotation() << m_anchor.x << m_anchor.y;
}

void Star::HandlePacket( PacketPtr packet ) {

}

void Star::SetOrbit( float velocity, float radius ) {
	SetVelocity( sf::Vector2f( velocity, radius ) );
}

void Star::SetAngle( float angle ) {
	m_angle = angle;
}

void Star::SetAnchor( float x, float y ) {
	m_anchor.x = x; m_anchor.y = y;
}
