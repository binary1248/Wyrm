#include <SFML/Graphics.hpp>

#include <config.hpp>
#include <game.hpp>
#include <network.hpp>
#include <objectmanager.hpp>
#include <particlesystem.hpp>
#include <objects/objects.hpp>
#include <utility.hpp>
#include <objects/star.hpp>

REGISTER_FACTORY( ObjectType::STAR, Star );

Star::Star( sf::Uint16 id, sf::Packet& packet ) :
	Object( ObjectType::STAR, id, packet ) {
  packet >> m_angle >> m_anchor.x >> m_anchor.y;

  CreateParticleSystem();

  glEnable( GL_LIGHTING );

	glEnable( GL_LIGHT0 );

	GLfloat light_ambient[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse );

	GLfloat light_position[] = { GetPosition().x, GetPosition().y, 0.0f, 1.0f };

	glLightfv( GL_LIGHT0, GL_POSITION, light_position );

	glDisable( GL_LIGHTING );
}

Star::~Star() {
}

void Star::CreateParticleSystem() {
	m_particle_system = Game::GetGame()->GetResourceManager()->GetParticleSystem( GetResourceId() );
	m_particle_system->SetPosition( GetPosition() );
  m_particle_system->Start( 100.0f );
}

void Star::Update( float time ) {
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

  if( GetPosition() != position ) {
		GLfloat light_position[] = { position.x, position.y, 0.0f, 1.0f };

		glLightfv( GL_LIGHT0, GL_POSITION, light_position );
  }

  SetPosition( position );

  m_particle_system->SetPosition( position );

  m_particle_system->Tick( time );
}

void Star::Draw( sf::RenderWindow& target ) {
  m_particle_system->Draw( target );
}

void Star::HandlePacket( sf::Packet& packet ) {
  sf::Uint16 type1;
  packet >> type1;

  switch(type1) {
    case ServerToClientObject::OBJECT_UPDATE: {
      Object::HandlePacket( packet );
      packet >> m_angle >> m_anchor.x >> m_anchor.y;
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
