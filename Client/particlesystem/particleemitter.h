#ifndef PARTICLEEMITTER_H_INCLUDED
#define PARTICLEEMITTER_H_INCLUDED

class Particle;
class ParticleSystem;

class ParticleEmitter {
  public:
    ParticleEmitter(ParticleSystem* ps);
    ~ParticleEmitter();

    void Tick(float secs);

    inline void SetPrototype(Particle* p) { prototype = p; }
    inline void SetSpread(float s) { spread = s; }
    inline void SetRadiusStart(float r) { radius_start = r; }
    inline void SetRadiusEnd(float r) { radius_end = r; }
    inline void SetRate(float r) { rate = r; }
  private:
    ParticleSystem* system;
    Particle* prototype;

    float spread;
    float radius_start;
    float radius_end;
    float rate;

    float queue;
};

#endif // PARTICLEEMITTER_H_INCLUDED
