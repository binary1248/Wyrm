#ifndef STAR_HPP_INCLUDED
#define STAR_HPP_INCLUDED

#include <objects/object.hpp>
#include <particlesystem.hpp>

class Star : public Object {
  public:
    Star( sf::Uint16 id, sf::Packet& packet );

    virtual ~Star();

    virtual void Update( float time );
    virtual void Draw( sf::RenderWindow& target );

    virtual void HandlePacket( sf::Packet& packet );
  private:
    void CreateParticleSystem();

		ParticleSystemPtr m_particle_system;

		sf::Vector2f m_anchor;
    float m_angle;
};

typedef std::shared_ptr<Star> StarPtr;
typedef std::weak_ptr<Star> StarWeakPtr;

#endif // STAR_HPP_INCLUDED
