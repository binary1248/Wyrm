#ifndef PARTICLESYSTEM_H_INCLUDED
#define PARTICLESYSTEM_H_INCLUDED

#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "particle.h"
#include "particleemitter.h"

class ParticleSystem : public boost::enable_shared_from_this<ParticleSystem> {
  public:
    ParticleSystem();
    ~ParticleSystem();

    void Draw(sf::RenderTarget& r);

    void Tick(float secs);
    void Start( float secs = 0.f );
    void Stop();

    inline void AddEmitter( ParticleEmitterPtr emitter ) { emitters.push_back( emitter ); }
    inline void AddParticle( ParticlePtr particle ) { particles.push_back( particle ); }

    inline sf::Vector2f GetPosition() { return position; }
    inline void SetPosition(sf::Vector2f p) { position = p; }
  private:
    bool running;

    sf::Vector2f position;

    std::vector<ParticleEmitterPtr> emitters;
    std::vector<ParticlePtr> particles;
};

typedef boost::shared_ptr<ParticleSystem> ParticleSystemPtr;

#endif // PARTICLESYSTEM_H_INCLUDED
