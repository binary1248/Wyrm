#ifndef BACKDROP_HPP_INCLUDED
#define BACKDROP_HPP_INCLUDED

#include <memory>

#include <SFML/Graphics.hpp>

#include <resourcemanager.hpp>

#define NUM_BACKDROP_PARTICLES 300

class System {
  public:
    System( std::shared_ptr<sf::RenderWindow> window, sf::Packet& packet,
							std::size_t num_particles = NUM_BACKDROP_PARTICLES );
    ~System();

    void Draw( sf::RenderWindow& target, float time );

  private:
		sf::Uint16 m_id;
		sf::String m_name;

		sf::Uint32 m_background_resource_id;

    sf::Vector2f m_backdrop_velocity;
    sf::Clock m_last_draw;

    TexturePtr m_texture;

    sf::Vector2f* m_particle_positions;
    GLfloat* m_particle_vertices;
    float* m_particle_velocities;

    std::size_t m_num_particles;

    GLuint m_particle_vbo;
    GLuint m_background_vbo;

    int m_width;
    int m_height;
};

typedef std::shared_ptr<System> SystemPtr;

#endif // BACKDROP_HPP_INCLUDED
