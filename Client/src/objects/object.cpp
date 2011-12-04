#include <config.hpp>
#include <objects/object.hpp>

Object::Object( sf::Uint16 type, sf::Uint16 id, sf::String name, sf::Vector2f position,
                sf::Vector2f velocity, float rotation, float rotational_velocity ) :
	m_type( type ),
	m_id( id ),
	m_name( name ),
	m_position( position ),
	m_velocity( velocity ),
	m_rotation( rotation ),
	m_rotational_velocity( rotational_velocity ),
	m_delete_me( false ) {

}

Object::Object( sf::Uint16 type, sf::Uint16 id, sf::Packet& packet ) :
	m_type( type ),
	m_id( id ) {
	packet >> m_name;
  packet >> m_position.x >> m_position.y;
  packet >> m_velocity.x >> m_velocity.y;
  packet >> m_rotation >> m_rotational_velocity;
}

Object::~Object() {

}

void Object::Update( float time ) {
	m_rotation += m_rotational_velocity * time;
	m_position += m_velocity * time;
}

void Object::Draw( sf::RenderWindow& /*target*/ ) {

}

void Object::HandlePacket( sf::Packet& packet ) {
  packet >> m_position.x >> m_position.y;
  packet >> m_velocity.x >> m_velocity.y;
  packet >> m_rotation >> m_rotational_velocity;
}

bool Object::IsDeleted() const {
	return m_delete_me;
}

void Object::Delete() {
	m_delete_me = true;
}

sf::Uint16 Object::GetType() const {
	return m_type;
}

void Object::SetType( sf::Uint16 type ) {
	m_type = type;
}

sf::Uint16 Object::GetID() const {
	return m_id;
}

void Object::SetID( sf::Uint16 id ) {
	m_id = id;
}

const sf::String& Object::GetName() const {
	return m_name;
}

void Object::SetName( const sf::String& name ) {
	m_name = name;
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
