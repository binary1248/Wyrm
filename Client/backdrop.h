#ifndef BACKDROP_H_INCLUDED
#define BACKDROP_H_INCLUDED

#include <SFML/Graphics.hpp>

#define NUM_BACKDROP_PARTICLES 100

void LoadBackdrop(sf::RenderWindow& w);
void DrawBackdrop(sf::RenderWindow& w);
void UpdateBackdrop(sf::Vector2f v);

#endif // BACKDROP_H_INCLUDED
