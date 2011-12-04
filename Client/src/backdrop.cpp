#include <cstdlib>

#include <random>

#include <SFML/Graphics.hpp>

#include <config.hpp>
#include <game.hpp>
#include <backdrop.hpp>

Backdrop::Backdrop( std::shared_ptr<sf::RenderWindow> window, std::size_t num_particles, unsigned int texture_size ) :
	m_texture_size( texture_size ),
	m_num_particles( num_particles ) {
  m_backdrop_velocity = sf::Vector2f( 0.0f, 0.0f );

  float amplitudes[] = { 30, 20, 20, 40, 10, 10, 2, 2 };

  m_texture = Game::GetGame()->GetResourceManager()->GetBackground( m_texture_size,
	                                                                  m_texture_size,
	                                                                  sf::Color( 90, 180, 180, 255 ),
	                                                                  4,
	                                                                  8,
	                                                                  amplitudes );
  m_background_sprite.SetTexture( *m_texture );

  std::mt19937 rng;
  std::uniform_real_distribution<float> u1( 0.0f, static_cast<float>( window->GetWidth() ) - 1.0f );
  std::uniform_real_distribution<float> u2( 0.0f, static_cast<float>( window->GetHeight() ) - 1.0f );
  std::uniform_real_distribution<float> u3( 0.60f, 0.75f );
  std::uniform_real_distribution<float> u4( 0.0f, 1.0f );

	m_particles = new sf::Shape[ m_num_particles ];
	m_particle_velocities = new float[ m_num_particles ];

  for( std::size_t index = 0; index < m_num_particles; ++index ) {
    m_particles[ index ] = sf::Shape::Circle( 0.0f,
                                              0.0f,
                                              u3( rng ),
                                              sf::Color(255, 255, 255, 85) );

		m_particles[ index ].SetPosition( u1( rng ), u2( rng ) );

		m_particle_velocities[ index ] = u4( rng );
  }
}

Backdrop::~Backdrop() {
	delete[] m_particles;
	delete[] m_particle_velocities;
}

void Backdrop::Draw( sf::RenderWindow& target, float time ) {
  float width = static_cast<float>( target.GetWidth() );
  float height = static_cast<float>( target.GetHeight() );

  float x_scale = static_cast<float>( width ) / static_cast<float>( m_texture_size );
  float y_scale = static_cast<float>( height ) / static_cast<float>( m_texture_size );

	m_background_sprite.SetPosition( m_backdrop_position - sf::Vector2f( width / 2.0f, height / 2.0f ) );
  m_background_sprite.SetScale( x_scale, y_scale );

  target.Draw( m_background_sprite );

  for( std::size_t index = 0; index < m_num_particles; ++index ) {
  	m_particles[ index ].Move( -m_backdrop_velocity * m_particle_velocities[ index ] * time );

		while( m_particles[ index ].GetPosition().x < m_backdrop_position.x - width / 2.0f ) {
      m_particles[ index ].Move( width, 0.0f );
    }

    while( m_particles[ index ].GetPosition().x >= m_backdrop_position.x + width / 2.0f ) {
      m_particles[ index ].Move( -width, 0.0f );
    }

    while( m_particles[ index ].GetPosition().y < m_backdrop_position.y - height / 2.0f ) {
      m_particles[ index ].Move( 0.0f, height );
    }

    while( m_particles[ index ].GetPosition().y >= m_backdrop_position.y + height / 2.0f ) {
      m_particles[ index ].Move( 0.0f, -height );
    }

    target.Draw( m_particles[ index ] );
  }

  m_last_draw.Reset();
}

void Backdrop::Update( const sf::Vector2f& velocity, const sf::Vector2f& position ) {
  m_backdrop_velocity = ( -velocity );
  m_backdrop_position = position;
}
