#include <SFML/Graphics.hpp>

#include "backdrop.h"

struct particle{
  sf::Vector2f position;
  float size;
};

sf::Image background;
sf::Sprite background_sprite;
sf::Vector2f backdrop_velocity = sf::Vector2f(0,0);
sf::Clock LastDraw;

particle particles[NUM_BACKDROP_PARTICLES];

void LoadBackdrop(sf::RenderWindow& w) {
  background.LoadFromFile("background.jpg");
  background_sprite.SetImage(background);

  for( unsigned int i = 0; i < NUM_BACKDROP_PARTICLES; i++) {
    particles[i].position.x = sf::Randomizer::Random(0.0f,(float)w.GetWidth());
    particles[i].position.y = sf::Randomizer::Random(0.0f,(float)w.GetHeight());
    particles[i].size = sf::Randomizer::Random(0.60f,0.75f);
  }
}

void DrawBackdrop(sf::RenderWindow& w) {
  unsigned int width = w.GetWidth();
  unsigned int height = w.GetHeight();

  sf::View view = w.GetView();
  w.SetView(w.GetDefaultView());
  w.Draw(background_sprite);

  for( unsigned int i = 0; i < NUM_BACKDROP_PARTICLES; i++) {
    particles[i].position += (backdrop_velocity * LastDraw.GetElapsedTime());
    if( particles[i].position.x < 0 ) {
      particles[i].position.x = width;
    }
    else if( particles[i].position.x >= width ) {
      particles[i].position.x = 0;
    }

    if( particles[i].position.y < 0 ) {
      particles[i].position.y = height;
    }
    else if( particles[i].position.y >= height ) {
      particles[i].position.y = 0;
    }
    w.Draw(sf::Shape::Circle(   particles[i].position.x,
                                particles[i].position.y,
                                particles[i].size,
                                sf::Color(255, 255, 255, 85)));
  }

  LastDraw.Reset();

  w.SetView(view);
}

void UpdateBackdrop(sf::Vector2f v) {
  backdrop_velocity = (-v);
}
