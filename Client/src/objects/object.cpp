#include <config.hpp>
#include <objects/object.hpp>

Object::Object( sf::Uint16 type, sf::Uint16 id, sf::Packet& packet ) :
	m_type( type ),
	m_id( id ),
	m_delete_me( false ) {
	packet >> m_name >> m_resource_id;
	packet >> m_size.x >> m_size.y;
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

const sf::Vector2f& Object::GetSize() const {
	return m_size;
}

void Object::SetSize( const sf::Vector2f& size ) {
	m_size = size;
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

void Object::SetResourceId( sf::Uint32 id ) {
	m_resource_id = id;
}

sf::Uint32 Object::GetResourceId() const {
	return m_resource_id;
}
