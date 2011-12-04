#ifndef PARTICLEEMITTER_HPP_INCLUDED
#define PARTICLEEMITTER_HPP_INCLUDED

#include <random>

class Particle;
class ParticleSystem;

typedef std::shared_ptr<ParticleSystem> ParticleSystemPtr;

class ParticleEmitter {
  public:
    ParticleEmitter();
    ~ParticleEmitter();

    void Tick( const ParticleSystemPtr& particle_system, float time );

    void SetPrototype( const ParticlePtr& particle );
    void SetSpread( float spread );
    void SetRadiusStart( float radius );
    void SetRadiusEnd( float radius );
    void SetRate( float rate );

  private:
    ParticlePtr m_prototype;

    float m_spread;
    float m_radius_start;
    float m_radius_end;
    float m_rate;

    float m_queue;

    static std::mt19937 rng;
};

typedef std::shared_ptr<ParticleEmitter> ParticleEmitterPtr;

#endif // PARTICLEEMITTER_HPP_INCLUDED
