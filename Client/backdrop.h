#ifndef BACKDROP_H_INCLUDED
#define BACKDROP_H_INCLUDED

#include <SFML/Graphics.hpp>

#define NUM_BACKDROP_PARTICLES 100

struct particle{
  sf::Vector2f position;
  float size;
};

class Backdrop {
  public:
    Backdrop(sf::RenderWindow& w);
    ~Backdrop();
    void Draw(sf::RenderWindow& w);
    void Update(sf::Vector2f v);
  private:
    sf::Image background;
    sf::Sprite background_sprite;
    sf::Vector2f backdrop_velocity;
    sf::Clock LastDraw;

    particle particles[NUM_BACKDROP_PARTICLES];
};

#endif // BACKDROP_H_INCLUDED
