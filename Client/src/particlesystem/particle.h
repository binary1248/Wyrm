#ifndef PARTICLE_H_INCLUDED
#define PARTICLE_H_INCLUDED

#include <boost/shared_ptr.hpp>

#include <SFML/Graphics.hpp>

#include "../resourcemanager.h"

class Particle {
  public:
    Particle( TexturePtr image );
    Particle( const boost::shared_ptr<Particle> p );
    ~Particle();

    void Tick(float ms);

    void Draw(sf::RenderTarget& r);

    inline void SetVisible(bool v) { visible = v; }
    inline void Kill() { alive = false; }
    inline bool IsAlive() { return alive; }

    void SetLifetime(float l);

    void SetSizeStart(sf::Vector2f s);
    void SetSizeEnd(sf::Vector2f s);

    void SetColorStart(sf::Color c);
    void SetColorEnd(sf::Color c);

    inline void SetPosition(sf::Vector2f p) { position = p; }

    inline sf::Vector2f GetVelocity() { return velocity; }
    inline void SetVelocity(sf::Vector2f v) { velocity = v; }
    inline sf::Vector2f GetAcceleration() { return acceleration; }
    inline void SetAcceleration(sf::Vector2f a) { acceleration = a; }

    void SetTexture(TexturePtr i);
  private:
    bool alive;
    bool visible;

    TexturePtr texture;

    float life;
    float lifetime;
    sf::Vector2f size_start;
    sf::Vector2f size_end;
    sf::Vector2f size_step;

		sf::Color color;
    sf::Color color_start;
    sf::Color color_end;

    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;

    sf::Sprite sprite;
};

typedef boost::shared_ptr<Particle> ParticlePtr;

#endif // PARTICLE_H_INCLUDED
