#include <iostream>

#include "../utility.h"
#include "../networkmanager.h"
#include "../player.h"
#include "../game.h"
#include "object.h"

Object::Object( sf::Uint16 type, sf::String name, sf::Vector2f position,
								sf::Vector2f velocity, float rotation, float rotational_velecity ) :
  m_type( type ),
  m_name( name ),
  m_position( position ),
  m_velocity( velocity ),
  m_rotation( rotation ),
  m_rotational_velocity( rotational_velecity ),
  m_delete_me( false ),
  m_id( Game::GetGame()->GetObjectManager()->NewID() ),
  m_dirty( true ),
  m_fresh( true ) {
  LogConsole( "Created object " + STRING_CAST( m_id ) + " of type " + STRING_CAST( m_type ) );
}

Object::~Object() {
  LogConsole( "Destroyed object " + STRING_CAST( m_id ) + " of type " + STRING_CAST( m_type ) );
}

void Object::Update( float time ) {
  if( IsDeleted() ) {
    return;
  }

  m_rotation += ( m_rotational_velocity * time );
  m_position += ( m_velocity * time );
}

void Object::FillPartialPacket( PacketPtr packet ) {
  (*packet) << static_cast<sf::Uint16>( SERVER_OBJECT ) << GetId() << static_cast<sf::Uint16>( OBJECT_UPDATE )
						<< GetPosition().x << GetPosition().y << GetRotation()
						<< GetVelocity().x << GetVelocity().y << GetRotationalVelocity();
}

void Object::FillFullPacket( PacketPtr packet ) {
  (*packet) << static_cast<sf::Uint16>( SERVER_OBJECT ) << GetId() << static_cast<sf::Uint16>( OBJECT_STATE )
						<< GetType()       << GetName()
						<< GetPosition().x << GetPosition().y
						<< GetVelocity().x << GetVelocity().y
						<< GetRotation()   << GetRotationalVelocity();
}

sf::Uint16 Object::GetId() {
	return m_id;
}
sf::Uint16 Object::GetType() {
	return m_type;
}
void Object::SetName( sf::String name ) {
	m_name = name;
}
sf::String Object::GetName() {
	return m_name;
}

bool Object::IsDirty() {
	return m_dirty;
}

void Object::SetDirty() {
	m_dirty = true;
}

void Object::ClearDirty() {
	m_dirty = false;
}

bool Object::IsFresh() {
	return m_fresh;
}

void Object::SetFresh() {
	m_fresh = true;
}

void Object::ClearFresh() {
	m_fresh = false;
}

void Object::Delete() {
	m_delete_me = true;
}

bool Object::IsDeleted() {
	return m_delete_me;
}

sf::Vector2f Object::GetPosition() {
	return m_position;
}

void Object::SetPosition( sf::Vector2f position ) {
	m_position = position;
}

sf::Vector2f Object::GetVelocity() {
	return m_velocity;
}

void Object::SetVelocity( sf::Vector2f velocity ) {
	m_velocity = velocity;
}

float Object::GetRotation() {
	return m_rotation;
}

void Object::SetRotation( float rotation ) {
	m_rotation = rotation;
}

float Object::GetRotationalVelocity() {
	return m_rotational_velocity;
}

void Object::SetRotationalVelocity( float rotational_velocity ) {
	m_rotational_velocity = rotational_velocity;
}
