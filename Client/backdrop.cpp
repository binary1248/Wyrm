#include <SFML/Graphics.hpp>
#include "game.h"
#include "backdrop.h"

#define TEX_SIZE 512

Backdrop::Backdrop(sf::RenderWindow& w) {
  backdrop_velocity = sf::Vector2f(0,0);

  float amplitudes[] = {30,20,20,20,10,10,0,0};

  image = 0;

  image = Game::GetGame()->GetResourceManager()->GetBackground(TEX_SIZE,
                                                               TEX_SIZE,
                                                               sf::Color(90,180,180,255),
                                                               4,
                                                               8,
                                                               (float*)amplitudes );
  background_sprite.SetImage(*image);

  for( unsigned int i = 0; i < NUM_BACKDROP_PARTICLES; i++) {
    particles[i].position.x = sf::Randomizer::Random(0.0f,(float)w.GetWidth());
    particles[i].position.y = sf::Randomizer::Random(0.0f,(float)w.GetHeight());
    particles[i].size = sf::Randomizer::Random(0.60f,0.75f);
  }
}

Backdrop::~Backdrop() {
  if( image ) {
    delete image;
  }
}

void Backdrop::Draw(sf::RenderWindow& w) {
  unsigned int width = w.GetWidth();
  unsigned int height = w.GetHeight();

  sf::View view = w.GetView();
  w.SetView(w.GetDefaultView());
  float x_scale = (float)width / TEX_SIZE;
  float y_scale = (float)height / TEX_SIZE;
  background_sprite.SetScale(x_scale, y_scale);
  w.Draw(background_sprite);

  for( unsigned int i = 0; i < NUM_BACKDROP_PARTICLES; i++) {
    particles[i].position += (backdrop_velocity * LastDraw.GetElapsedTime());

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

    w.Draw(sf::Shape::Circle( particles[i].position.x,
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
