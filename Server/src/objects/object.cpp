#include <config.hpp>
#include <utility.hpp>
#include <networkmanager.hpp>
#include <player.hpp>
#include <game.hpp>
#include <objects/object.hpp>

Object::Object( sf::Uint16 type, sf::String name, sf::Vector2f position,
								sf::Vector2f velocity, float rotation, float rotational_velecity ) :
  m_id( Game::GetGame()->GetObjectManager()->NewID() ),
  m_type( type ),
  m_name( name ),
  m_position( position ),
  m_velocity( velocity ),
  m_rotation( rotation ),
  m_rotational_velocity( rotational_velecity ),
  m_delete_me( false ),
  m_dirty( true ),
  m_fresh( true ) {
  LogConsole( "Created object " + string_cast( m_id ) + " of type " + string_cast( m_type ) );
}

Object::~Object() {
  LogConsole( "Destroyed object " + string_cast( m_id ) + " of type " + string_cast( m_type ) );
}

void Object::Update( float time ) {
  if( IsDeleted() ) {
    return;
  }

  m_rotation += ( m_rotational_velocity * time );
  m_position += ( m_velocity * time );
}

void Object::FillPartialPacket( const PacketPtr& packet ) {
  (*packet) << static_cast<sf::Uint16>( SERVER_OBJECT ) << GetId() << static_cast<sf::Uint16>( OBJECT_UPDATE )
						<< GetPosition().x << GetPosition().y
						<< GetVelocity().x << GetVelocity().y
	          << GetRotation() << GetRotationalVelocity();
}

void Object::FillFullPacket( const PacketPtr& packet ) {
  (*packet) << static_cast<sf::Uint16>( SERVER_OBJECT ) << GetId() << static_cast<sf::Uint16>( OBJECT_STATE )
						<< GetType()       << GetName()
						<< GetPosition().x << GetPosition().y
						<< GetVelocity().x << GetVelocity().y
						<< GetRotation()   << GetRotationalVelocity();
}

sf::Uint16 Object::GetId() const {
	return m_id;
}

sf::Uint16 Object::GetType() const {
	return m_type;
}

void Object::SetName( const sf::String& name ) {
	m_name = name;
}

const sf::String& Object::GetName() const {
	return m_name;
}

bool Object::IsDirty() const {
	return m_dirty;
}

void Object::SetDirty() {
	m_dirty = true;
}

void Object::ClearDirty() {
	m_dirty = false;
}

bool Object::IsFresh() const {
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

bool Object::IsDeleted() const {
	return m_delete_me;
}

const sf::Vector2f& Object::GetPosition() const {
	return m_position;
}

void Object::SetPosition( const sf::Vector2f& position ) {
	m_position = position;
}

const sf::Vector2f& Object::GetVelocity() const {
	return m_velocity;
}

void Object::SetVelocity( const sf::Vector2f& velocity ) {
	m_velocity = velocity;
}

float Object::GetRotation() const {
	return m_rotation;
}

void Object::SetRotation( float rotation ) {
	m_rotation = rotation;
}

float Object::GetRotationalVelocity() const {
	return m_rotational_velocity;
}

void Object::SetRotationalVelocity( float rotational_velocity ) {
	m_rotational_velocity = rotational_velocity;
}
