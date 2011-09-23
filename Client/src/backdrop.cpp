#include <cstdlib>
#include <iostream>

#include <boost/random.hpp>

#include <SFML/Graphics.hpp>

#include "game.h"
#include "backdrop.h"

Backdrop::Backdrop( boost::shared_ptr<sf::RenderWindow> w, std::size_t num_particles, std::size_t texture_size ) :
	m_texture_size( texture_size ),
	m_num_particles( num_particles ) {
  backdrop_velocity = sf::Vector2f(0,0);

  float amplitudes[] = {30,20,20,40,10,10,2,2};

  texture = Game::GetGame()->GetResourceManager()->GetBackground(m_texture_size,
																																 m_texture_size,
																																 sf::Color(90,180,180,255),
																																 4,
																																 8,
																																 (float*)amplitudes );
  background_sprite.SetTexture(*texture);

  boost::lagged_fibonacci607 rng;
  boost::uniform_real<float> u1(0.0f, (float)( w->GetWidth() ) - 1 );
  boost::uniform_real<float> u2(0.0f, (float)( w->GetHeight() ) - 1 );
  boost::uniform_real<float> u3(0.60f, 0.75f);
  boost::variate_generator<boost::lagged_fibonacci607&, boost::uniform_real<float> > gen1(rng, u1);
  boost::variate_generator<boost::lagged_fibonacci607&, boost::uniform_real<float> > gen2(rng, u2);
  boost::variate_generator<boost::lagged_fibonacci607&, boost::uniform_real<float> > gen3(rng, u3);

  particles.reset( new BackdropParticle[m_num_particles] );

  for( unsigned int i = 0; i < NUM_BACKDROP_PARTICLES; i++) {
    particles[i].position.x = gen1();
    particles[i].position.y = gen2();
    particles[i].size = gen3();
  }
}

Backdrop::~Backdrop() {
}

void Backdrop::Draw(sf::RenderWindow& w) {
  float width = (float)(w.GetWidth());
  float height = (float)(w.GetHeight());

  sf::View view = w.GetView();
  w.SetView(w.GetDefaultView());
  float x_scale = width / m_texture_size;
  float y_scale = height / m_texture_size;
  background_sprite.SetScale(x_scale, y_scale);
  w.Draw(background_sprite);

  for( unsigned int i = 0; i < NUM_BACKDROP_PARTICLES; i++) {
    particles[i].position += (backdrop_velocity * (float)(LastDraw.GetElapsedTime()) / 1000.0f );

    while( particles[i].position.x < 0 ) {
      particles[i].position.x += width;
    }

    while( particles[i].position.x >= width ) {
      particles[i].position.x -= width;
    }

    while( particles[i].position.y < 0 ) {
      particles[i].position.y += height;
    }

    while( particles[i].position.y >= height ) {
      particles[i].position.y -= height;
    }

    w.Draw( sf::Shape::Circle( particles[i].position.x,
                               particles[i].position.y,
                               particles[i].size,
                               sf::Color(255, 255, 255, 85) ) );

  }

  LastDraw.Reset();

  w.SetView(view);
}

void Backdrop::Update(sf::Vector2f v) {
  backdrop_velocity = (-v);
}
