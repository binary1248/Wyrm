#ifndef BACKDROP_H_INCLUDED
#define BACKDROP_H_INCLUDED

#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

#include <SFML/Graphics.hpp>

#define NUM_BACKDROP_PARTICLES 100
#define TEX_SIZE 512

struct BackdropParticle{
  sf::Vector2f position;
  float size;
};

class Backdrop {
  public:
    Backdrop( boost::shared_ptr<sf::RenderWindow> w,
							std::size_t num_particles = NUM_BACKDROP_PARTICLES,
							std::size_t tex_size = TEX_SIZE );
    ~Backdrop();
    void Draw(sf::RenderWindow& w);
    void Update(sf::Vector2f v);
  private:
    //sf::Image background;
    sf::Sprite background_sprite;
    sf::Vector2f backdrop_velocity;
    sf::Clock LastDraw;

		std::size_t m_texture_size;
    TexturePtr texture;

    boost::shared_array<BackdropParticle> particles;
    std::size_t m_num_particles;
};

typedef boost::shared_ptr<Backdrop> BackdropPtr;

#endif // BACKDROP_H_INCLUDED
