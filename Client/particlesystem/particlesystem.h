#ifndef PARTICLESYSTEM_H_INCLUDED
#define PARTICLESYSTEM_H_INCLUDED

#include <vector>

#include "particle.h"
#include "particleemitter.h"

class ParticleSystem {
  public:
    ParticleSystem();
    ~ParticleSystem();

    void Draw(sf::RenderTarget& r);

    void Tick(float secs);
    void Start(float secs = 0);
    void Stop();

    inline void AddEmitter(ParticleEmitter* pe) { emitters.push_back(pe); }
    inline void AddParticle(Particle* p) { particles.push_back(p); }

    inline sf::Vector2f GetPosition() { return position; }
    inline void SetPosition(sf::Vector2f p) { position = p; }
  private:
    bool running;

    sf::Vector2f position;

    std::vector<ParticleEmitter*> emitters;
    std::vector<Particle*> particles;
};

#endif // PARTICLESYSTEM_H_INCLUDED
