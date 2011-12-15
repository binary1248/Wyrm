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
	m_angle( 0.0f ),
	m_sphere( 0 ),
	m_sphere_list_id( 0 ) {
  Init();
}

Planet::Planet( sf::Uint16 id, sf::Packet& packet ) :
	Object( PLANET, id, packet ) {
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
  GradientPoint points[8] = {
		{ -1.0000f,   0,   0, 128, 255 }, // deeps
		{ -0.2500f,   0,   0, 255, 255 }, // shallow
		{  0.0000f,   0, 128, 255, 255 }, // shore
		{  0.0625f, 240, 240,  64, 255 }, // sand
		{  0.1250f,  32, 160,   0, 255 }, // grass
		{  0.3750f, 224, 224,   0, 255 }, // dirt
		{  0.7500f, 128, 128, 128, 255 }, // rock
		{  1.0000f, 255, 255, 255, 255 }  // snow
  };

  m_texture = Game::GetGame()->GetResourceManager()->GetPlanet( 256,
                                                                8,
                                                                1.0f,
                                                                0.5f,
                                                                1.0f,
                                                                1.0f,
                                                                points,
                                                                8 );

	m_sphere = gluNewQuadric();
	gluQuadricTexture( m_sphere, GL_TRUE );

	m_sphere_list_id = glGenLists( 1 );

	glNewList( m_sphere_list_id, GL_COMPILE );

	glColor4f( 1.f, 1.f, 1.f, 1.f );
	gluSphere( m_sphere, 100.0, 64, 64 );

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

void Planet::Draw( sf::RenderWindow& target ) {
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();

	glViewport( 0, 0, target.GetWidth(), target.GetHeight() );
	glOrtho( 0.0f, target.GetWidth(), target.GetHeight(), 0.0f, -1000.0f, 1000.0f );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();

	m_texture->Bind();

	glTranslatef( ( GetPosition().x + static_cast<float>( target.GetWidth() ) / 2 - target.GetView().GetCenter().x ),
								( GetPosition().y + static_cast<float>( target.GetHeight() ) / 2 - target.GetView().GetCenter().y ), 0.0f );

	glRotatef( GetRotation(), 0.0f, 0.0f, 1.0f );

	glCallList( m_sphere_list_id );

	glPopMatrix();

	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
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
