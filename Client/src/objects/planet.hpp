#ifndef PLANET_HPP_INCLUDED
#define PLANET_HPP_INCLUDED

#include <GL/glu.h>

#include <resourcemanager.hpp>
#include <objects/object.hpp>

class Planet : public Object {
  public:
    Planet( sf::Uint16 id, sf::String name, sf::Vector2f position,
		        sf::Vector2f velocity, float rotation, float rotational_velocity );

    Planet( sf::Uint16 id, sf::Packet& packet );

    virtual ~Planet();

    virtual void Update( float time );

    virtual void Draw( sf::RenderWindow& target );

    virtual void HandlePacket( sf::Packet& packet );

  private:
    void Init();

    float m_angle;
    sf::Vector2f m_anchor;

    TexturePtr m_texture;

    GLUquadricObj *m_sphere;
    GLuint m_sphere_list_id;
};

typedef std::shared_ptr<Planet> PlanetPtr;
typedef std::weak_ptr<Planet> PlanetWeakPtr;

#endif // PLANET_HPP_INCLUDED
