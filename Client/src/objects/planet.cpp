#include <GL/glu.h>

#include <SFML/Graphics.hpp>

#include <config.hpp>
#include <game.hpp>
#include <network.hpp>
#include <objects/objects.hpp>
#include <objectmanager.hpp>
#include <utility.hpp>
#include <objects/planet.hpp>

REGISTER_FACTORY( PLANET, Planet );

Planet::Planet( sf::Uint16 id, sf::String name, sf::Vector2f position,
                sf::Vector2f velocity, float rotation, float rotational_velocity ) :
	Object( PLANET, id, name, position, velocity, rotation, rotational_velocity ),
	m_angle( 0.0f ) {
  Init();
}

Planet::Planet( sf::Uint16 id, sf::Packet& packet ) :
	Object( PLANET, id, packet ) {
  packet >> m_angle >> m_anchor.x >> m_anchor.y;

  Init();
}

Planet::~Planet() {
}

void Planet::Init() {
  float amplitudes[] = { 100.0f, 60.0f, 10.0f, 80.0f, 0.0f, 10.0f, 30.0f, 40.0f };

  m_texture = Game::GetGame()->GetResourceManager()->GetPlanet( 100,
	                                                              100,
                                                                sf::Color( 0xDB, 0x99, 0x00, 255 ),
                                                                4,
                                                                8,
                                                                amplitudes );

  m_sprite.SetTexture( *m_texture );
  m_sprite.SetOrigin( static_cast<float>( m_texture->GetWidth() ) / 2.f, static_cast<float>( m_texture->GetHeight() ) / 2.f );
  m_sprite.SetScale( 1.6f, 1.6f );
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

  m_sprite.SetPosition( position );
  m_sprite.SetRotation( GetRotation() );
}

void Planet::Draw( sf::RenderWindow& target ) {
  target.Draw( m_sprite );
/*
  target.SaveGLStates();

	gluPerspective( 90.f, 1.f, 0.f, 100.f );
	//glMatrixMode( GL_PROJECTION );
	//glOrtho( 0, target.GetWidth(), 0, target.GetHeight(), 0, 100 );
	//glMatrixMode( GL_MODELVIEW );
	m_texture->Bind();
	glColor4f( 1.f, 1.f, 1.f, 1.f );

	GLUquadricObj *sphere = NULL;
	sphere = gluNewQuadric();
	gluQuadricTexture(sphere, GL_TRUE);
	glPushMatrix();
	//glLoadIdentity();
	//glTranslatef( ( GetPosition().x - target.GetView().GetCenter().x ) / 100, ( GetPosition().y - target.GetView().GetCenter().y ) / 100, -20.0f );
	glTranslatef( 0, 0, -20 );
	gluSphere( sphere, 2.0, 10, 10 );
	glPopMatrix();
	gluDeleteQuadric(sphere);

	target.RestoreGLStates();*/
}

void Planet::HandlePacket( sf::Packet& packet ) {
  sf::Uint16 type1;
  packet >> type1;

  switch(type1) {
    case OBJECT_UPDATE: {
      Object::HandlePacket( packet );
      packet >> m_angle;
      break;
    }
    case OBJECT_STATE: {
      sf::Uint16 type;
      sf::String name;
      packet >> type >> name;
      assert( type == GetType() );
      SetName( name );
      Object::HandlePacket( packet );
      packet >> m_angle >> m_anchor.x >> m_anchor.y;
      break;
    }
    case OBJECT_REMOVE: {
      Delete();
      break;
    }
  }
}
