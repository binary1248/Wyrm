#ifndef PARTICLEEMITTER_H_INCLUDED
#define PARTICLEEMITTER_H_INCLUDED

#include <boost/shared_ptr.hpp>

class Particle;
class ParticleSystem;

typedef boost::shared_ptr<ParticleSystem> ParticleSystemPtr;

class ParticleEmitter {
  public:
    ParticleEmitter();
    ~ParticleEmitter();

    void Tick( ParticleSystemPtr system, float secs );

    inline void SetPrototype( ParticlePtr p ) { prototype = p; }
    inline void SetSpread(float s) { spread = s; }
    inline void SetRadiusStart(float r) { radius_start = r; }
    inline void SetRadiusEnd(float r) { radius_end = r; }
    inline void SetRate(float r) { rate = r; }
  private:
    ParticlePtr prototype;

    float spread;
    float radius_start;
    float radius_end;
    float rate;

    float queue;
};

typedef boost::shared_ptr<ParticleEmitter> ParticleEmitterPtr;

#endif // PARTICLEEMITTER_H_INCLUDED
