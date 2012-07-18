#include <GL/glu.h>

#include <SFML/Graphics.hpp>

#include <config.hpp>
#include <game.hpp>
#include <network.hpp>
#include <objects/objects.hpp>
#include <objectmanager.hpp>
#include <utility.hpp>
#include <objects/planet.hpp>

REGISTER_FACTORY( ObjectType::PLANET, Planet );

Planet::Planet( sf::Uint16 id, sf::Packet& packet ) :
	Object( ObjectType::PLANET, id, packet ),
	m_sphere( 0 ),
	m_sphere_list_id( 0 ) {
	packet >> m_angle >> m_anchor.x >> m_anchor.y;

	Init();
}

Planet::~Planet() {
	if( m_sphere ) {
		gluDeleteQuadric( m_sphere );
	}

	glDeleteLists( m_sphere_list_id, 1 );
}

void Planet::Init() {
	m_texture = Game::GetGame()->GetResourceManager()->GetTexture( GetResourceId() );

	m_sphere = gluNewQuadric();
	gluQuadricTexture( m_sphere, GL_TRUE );

	m_sphere_list_id = glGenLists( 1 );

	glNewList( m_sphere_list_id, GL_COMPILE );

	glColor4f( 1.f, 1.f, 1.f, 1.f );
	gluSphere( m_sphere, std::max( GetSize().x, GetSize().y ), 128, 128 );

	glEndList();
}

void Planet::Update( float time ) {
	m_angle += GetVelocity().x * time;

	while( m_angle > 360 ) {
		m_angle -= 360;
	}
	while( m_angle < (-360) ) {
		m_angle += 360;
	}

	Object::Update( time );

	sf::Vector2f position;

	position.x = static_cast<float>( cos( clean_angle( m_angle ) ) ) * GetVelocity().y;
	position.y = static_cast<float>( sin( clean_angle( m_angle ) ) ) * GetVelocity().y;

	position += m_anchor;
	SetPosition( position );
}

void Planet::Draw( sf::RenderWindow& /*target*/ ) {
	glPushMatrix();

	glEnable( GL_LIGHTING );

	m_texture->bind();

	glTranslatef( ( GetPosition().x ),
								( GetPosition().y ),
								0.0f );

	glRotatef( GetRotation(), 0.0f, 0.0f, 1.0f );

	glCallList( m_sphere_list_id );

	glDisable( GL_LIGHTING );

	glPopMatrix();
}

void Planet::HandlePacket( sf::Packet& packet ) {
	sf::Uint16 type1;
	packet >> type1;

	switch(type1) {
		case ServerToClientObject::OBJECT_UPDATE: {
			Object::HandlePacket( packet );
			packet >> m_angle;
			break;
		}
		case ServerToClientObject::OBJECT_STATE: {
			sf::Uint16 type;
			sf::String name;
			sf::Uint32 resource_id;
			sf::Vector2f size;
			packet >> type >> name >> resource_id >> size.x >> size.y;
			assert( type == GetType() );
			SetName( name );
			SetResourceId( resource_id );
			SetSize( size );
			Object::HandlePacket( packet );
			packet >> m_angle >> m_anchor.x >> m_anchor.y;
			break;
		}
		case ServerToClientObject::OBJECT_REMOVE: {
			Delete();
			break;
		}
	}
}
