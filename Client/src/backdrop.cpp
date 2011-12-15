#include <cstdlib>

#include <random>

#include <SFML/Graphics.hpp>

#include <config.hpp>
#include <game.hpp>
#include <backdrop.hpp>

Backdrop::Backdrop( std::shared_ptr<sf::RenderWindow> window, std::size_t num_particles ) :
	m_num_particles( num_particles ),
	m_width( window->GetWidth() ),
	m_height( window->GetHeight() ) {
  m_backdrop_velocity = sf::Vector2f( 0.0f, 0.0f );

  GradientPoint points[2] = {
		{ -1.0000f, 0,  0,  0, 255 },
		{  1.0000f, 0, 60, 80, 255 }
  };

  m_texture = Game::GetGame()->GetResourceManager()->GetBackground( m_width,
	                                                                  m_height,
	                                                                  4,
	                                                                  1.0f,
	                                                                  0.5f,
	                                                                  1.0f,
	                                                                  1.0f,
	                                                                  points,
	                                                                  2 );
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

  float x_scale = static_cast<float>( width ) / static_cast<float>( m_width );
  float y_scale = static_cast<float>( height ) / static_cast<float>( m_height );

  m_background_sprite.SetScale( x_scale, y_scale );

  sf::View original_view = target.GetView();
  target.SetView( target.GetDefaultView() );

  target.Draw( m_background_sprite );

  for( std::size_t index = 0; index < m_num_particles; ++index ) {
  	m_particles[ index ].Move( m_backdrop_velocity * m_particle_velocities[ index ] * time );

		while( m_particles[ index ].GetPosition().x < 0.0f ) {
      m_particles[ index ].Move( width, 0.0f );
    }

    while( m_particles[ index ].GetPosition().x >= width ) {
      m_particles[ index ].Move( -width, 0.0f );
    }

    while( m_particles[ index ].GetPosition().y < 0.0f ) {
      m_particles[ index ].Move( 0.0f, height );
    }

    while( m_particles[ index ].GetPosition().y >= height ) {
      m_particles[ index ].Move( 0.0f, -height );
    }

    target.Draw( m_particles[ index ] );
  }

  target.SetView( original_view );

  m_last_draw.Reset();
}

void Backdrop::Update( const sf::Vector2f& velocity ) {
  m_backdrop_velocity = ( -velocity );
}
