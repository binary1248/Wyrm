#ifndef BACKDROP_HPP_INCLUDED
#define BACKDROP_HPP_INCLUDED

#include <memory>

#include <SFML/Graphics.hpp>

#include <resourcemanager.hpp>

#define NUM_BACKDROP_PARTICLES 200
#define TEX_SIZE 512

class Backdrop {
  public:
    Backdrop( std::shared_ptr<sf::RenderWindow> window,
							std::size_t num_particles = NUM_BACKDROP_PARTICLES );
    ~Backdrop();

    void Draw( sf::RenderWindow& target, float time );
    void Update( const sf::Vector2f& velocity );

  private:
    sf::Sprite m_background_sprite;
    sf::Vector2f m_backdrop_velocity;
    sf::Clock m_last_draw;

    TexturePtr m_texture;

    sf::Shape* m_particles;
    float* m_particle_velocities;
    std::size_t m_num_particles;

    int m_width;
    int m_height;
};

typedef std::shared_ptr<Backdrop> BackdropPtr;

#endif // BACKDROP_HPP_INCLUDED
