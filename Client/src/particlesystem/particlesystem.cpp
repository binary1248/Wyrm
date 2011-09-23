#include <iostream>

#include <boost/foreach.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "particle.h"
#include "particleemitter.h"
#include "particlesystem.h"

ParticleSystem::ParticleSystem() {
  running = false;
}

ParticleSystem::~ParticleSystem() {
  Stop();
}

void ParticleSystem::Draw( sf::RenderTarget& target ) {
	BOOST_FOREACH( ParticlePtr particle, particles ) {
		particle->Draw( target );
	}
}

void ParticleSystem::Tick( float secs ) {
  if( !running ) {
    return;
  }

	BOOST_FOREACH( ParticleEmitterPtr emitter, emitters ) {
		emitter->Tick( shared_from_this(), secs );
	}

  for( std::vector<ParticlePtr>::iterator iter = particles.begin(); iter != particles.end(); ) {
    if( !(*iter)->IsAlive() ) {
      iter = particles.erase( iter );
    }

    (*iter)->Tick(secs);
    iter++;
  }
}

void ParticleSystem::Start( float secs ) {
  running = true;

  if( secs <= 0.f ) {
    return;
  }

  for( float f = 0; f < secs; f+=1 ) {
    Tick(1);
  }
}

void ParticleSystem::Stop() {
  running = false;

  particles.clear();
}
