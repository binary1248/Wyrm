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
							std::size_t num_particles = NUM_BACKDROP_PARTICLES,
							unsigned int tex_size = TEX_SIZE );
    ~Backdrop();

    void Draw( sf::RenderWindow& target, float time );
    void Update( const sf::Vector2f& velocity, const sf::Vector2f& position );

  private:
    sf::Sprite m_background_sprite;
    sf::Vector2f m_backdrop_velocity;
    sf::Vector2f m_backdrop_position;
    sf::Clock m_last_draw;

		unsigned int m_texture_size;
    TexturePtr m_texture;

    sf::Shape* m_particles;
    float* m_particle_velocities;
    std::size_t m_num_particles;
};

typedef std::shared_ptr<Backdrop> BackdropPtr;

#endif // BACKDROP_HPP_INCLUDED
