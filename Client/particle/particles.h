#ifndef PARTICLESYSTEM_H_INCLUDED
#define PARTICLESYSTEM_H_INCLUDED

#include "ParticleSystem.h"

//Particle systems are built up with emitters. so let's make a basic point emitter
class CMyEmitter : public part::IParticleEmitter
{
public:
	virtual ~CMyEmitter() {}

protected:
	virtual void InitParticle(ParticlePtr particle);
};

//Just an emitter i kind of boring so let's make something that can affect the particles when they are in flight
//A scale in affector is a simple thing to make that makes the system look a lot better.
class CScaleInAffector : public part::IParticleAffector
{
public:
	CScaleInAffector(float fadeTime, float beginScale, float endScale);

	void Update(part::CParticle *particle, float dt);

private:
	float mFadeTime;
	float mSpeed;
	float mMaxScale;
};

#endif // PARTICLESYSTEM_H_INCLUDED
