#include <cstdlib>

#include <random>

#include <GL/glew.h>

#include <SFML/Graphics.hpp>

#include <config.hpp>
#include <game.hpp>
#include <system.hpp>

System::System( std::shared_ptr<sf::RenderWindow> window, sf::Packet& packet, std::size_t num_particles ) :
	m_num_particles( num_particles ),
	m_width( window->getSize().x ),
	m_height( window->getSize().y ) {
	m_backdrop_velocity = sf::Vector2f( 0.0f, 0.0f );

	packet >> m_id;
	packet >> m_name;
	packet >> m_background_resource_id;

	m_texture = Game::GetGame()->GetResourceManager()->GetTexture( 1 );

	std::mt19937 rng;
	std::uniform_real_distribution<float> position_x_distribution( 0.0f, static_cast<float>( window->getSize().x ) - 1.0f );
	std::uniform_real_distribution<float> position_y_distribution( 0.0f, static_cast<float>( window->getSize().y ) - 1.0f );
	std::uniform_real_distribution<float> velocity_distribution( 0.0f, 0.8f );

	m_particle_positions = new sf::Vector2f[ m_num_particles ];
	m_particle_vertices = new GLfloat[ m_num_particles * 2 ];
	m_particle_velocities = new float[ m_num_particles ];

	for( std::size_t index = 0; index < m_num_particles; ++index ) {
		m_particle_positions[ index ].x = position_x_distribution( rng );
		m_particle_positions[ index ].y = position_y_distribution( rng );

		m_particle_velocities[ index ] = velocity_distribution( rng );
	}

	glGenBuffers( 1, &m_particle_vbo );
	glGenBuffers( 1, &m_background_vbo );
}

System::~System() {
	glDeleteBuffers( 1, &m_background_vbo );
	glDeleteBuffers( 1, &m_particle_vbo );

	delete[] m_particle_positions;
	delete[] m_particle_vertices;
	delete[] m_particle_velocities;
}

void System::Draw( sf::RenderWindow& target, float time ) {
	float width = static_cast<float>( target.getSize().x );
	float height = static_cast<float>( target.getSize().y );

	glColor3f( 1.0f, 1.0f, 1.0f );

	ObjectPtr agent = Game::GetGame()->GetPlayer()->GetAgent();

	if( !agent ) {
		return;
	}

	if( m_texture ) {
		m_texture->bind();

		glShadeModel( GL_FLAT );
		glDisable( GL_BLEND );
		glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

		GLfloat background_buffer_data[] = {
			0.0f, 0.0f,
			agent->GetPosition().x - static_cast<float>( target.getSize().x ) / 2.0f,
			agent->GetPosition().y - static_cast<float>( target.getSize().y ) / 2.0f,
			0.0f, 1.0f,
			agent->GetPosition().x - static_cast<float>( target.getSize().x ) / 2.0f,
			agent->GetPosition().y + static_cast<float>( target.getSize().y ) / 2.0f,
			1.0f, 1.0f,
			agent->GetPosition().x + static_cast<float>( target.getSize().x ) / 2.0f,
			agent->GetPosition().y + static_cast<float>( target.getSize().y ) / 2.0f,
			1.0f, 0.0f,
			agent->GetPosition().x + static_cast<float>( target.getSize().x ) / 2.0f,
			agent->GetPosition().y - static_cast<float>( target.getSize().y ) / 2.0f
		};

		glBindBuffer( GL_ARRAY_BUFFER, m_background_vbo );
		glBufferData( GL_ARRAY_BUFFER, 4 * 4 * sizeof( GLfloat ), background_buffer_data, GL_STREAM_DRAW );

		glTexCoordPointer( 2, GL_FLOAT, 4 * sizeof( GLfloat ), 0 );
		glVertexPointer( 2, GL_FLOAT, 4 * sizeof( GLfloat ), reinterpret_cast<GLvoid*>( 2 * sizeof( GLfloat ) ) );

		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		glEnableClientState( GL_VERTEX_ARRAY );

		glDrawArrays( GL_QUADS, 0, 4 );

		glDisableClientState( GL_VERTEX_ARRAY );
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );

		glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		glEnable( GL_BLEND );
		glShadeModel( GL_SMOOTH );
	}

	glColor4f( 1.0f, 1.0f, 1.0f, 0.4f );

	glDisable( GL_TEXTURE_2D );

	for( std::size_t index = 0; index < m_num_particles; ++index ) {
		m_particle_positions[ index ] += ( -agent->GetVelocity() ) * m_particle_velocities[ index ] * time;

		while( m_particle_positions[ index ].x < 0.0f ) {
			m_particle_positions[ index ].x += width;
		}

		while( m_particle_positions[ index ].x >= width ) {
			m_particle_positions[ index ].x -= width;
		}

		while( m_particle_positions[ index ].y < 0.0f ) {
			m_particle_positions[ index ].y += height;
		}

		while( m_particle_positions[ index ].y >= height ) {
			m_particle_positions[ index ].y -= height;
		}

		m_particle_vertices[ index * 2 + 0 ] = std::floor( m_particle_positions[ index ].x +
		                                       agent->GetPosition().x -
		                                       static_cast<float>( target.getSize().x ) / 2.0f ) + 0.5f;
		m_particle_vertices[ index * 2 + 1 ] = std::floor( m_particle_positions[ index ].y +
		                                       agent->GetPosition().y -
		                                       static_cast<float>( target.getSize().y ) / 2.0f ) + 0.5f;
	}

	glBindBuffer( GL_ARRAY_BUFFER, m_particle_vbo );
	glBufferData( GL_ARRAY_BUFFER, 0, 0, GL_STREAM_DRAW );
	glBufferData( GL_ARRAY_BUFFER, m_num_particles * 2 * sizeof( GLfloat ), m_particle_vertices, GL_STREAM_DRAW );

	glVertexPointer( 2, GL_FLOAT, 0, 0 );

	glEnableClientState( GL_VERTEX_ARRAY );

	glDrawArrays( GL_POINTS, 0, m_num_particles );

	glDisableClientState( GL_VERTEX_ARRAY );

	glEnable( GL_TEXTURE_2D );
}
