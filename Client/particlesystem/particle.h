#ifndef PARTICLE_H_INCLUDED
#define PARTICLE_H_INCLUDED

#include <SFML/Graphics.hpp>

class Particle {
  public:
    Particle( sf::Image* image );
    Particle(const Particle& p);
    ~Particle();

    void Tick(float ms);

    void Draw(sf::RenderTarget& r);

    inline void SetVisible(bool v) { visible = v; }
    inline void Kill() { alive = false; }
    inline bool IsAlive() { return alive; }

    inline void SetLifetime(float l) { lifetime = l; }

    inline void SetSizeStart(sf::Vector2f s) { size_start = s; }
    inline void SetSizeEnd(sf::Vector2f s) { size_end = s; }

    inline void SetColorStart(sf::Color c) { color_start = c; }
    inline void SetColorEnd(sf::Color c) { color_end = c; }

    inline void SetPosition(sf::Vector2f p) { position = p; }

    inline sf::Vector2f GetVelocity() { return velocity; }
    inline void SetVelocity(sf::Vector2f v) { velocity = v; }
    inline sf::Vector2f GetAcceleration() { return acceleration; }
    inline void SetAcceleration(sf::Vector2f a) { acceleration = a; }

    void SetImage(sf::Image* i);
  private:
    bool alive;
    bool visible;

    sf::Image* image;

    float life;
    float lifetime;
    sf::Vector2f size_start;
    sf::Vector2f size_end;
    sf::Color color_start;
    sf::Color color_end;
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;

    sf::Sprite sprite;
};

#endif // PARTICLE_H_INCLUDED
