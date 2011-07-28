#include <iostream>
#include "particle.h"
#include "particleemitter.h"
#include "particlesystem.h"

ParticleSystem::ParticleSystem() {
  running = false;
}

ParticleSystem::~ParticleSystem() {
  Stop();

  while( !emitters.empty() ) {
    delete emitters.back();
    emitters.pop_back();
  }
}

void ParticleSystem::Draw(sf::RenderTarget& r) {
  for( std::vector<Particle*>::iterator i = particles.begin(); i != particles.end(); i++ ) {
    (*i)->Draw(r);
  }
}

void ParticleSystem::Tick(float secs) {
  if( !running ) {
    return;
  }

  for( size_t i = 0; i < emitters.size(); i++ ) {
    emitters[i]->Tick(secs);
  }

  for( std::vector<Particle*>::iterator i = particles.begin(); i != particles.end(); ) {
    if( !(*i)->IsAlive() ) {
      delete (*i);
      i = particles.erase(i);
    }

    (*i)->Tick(secs);
    i++;
  }
}

void ParticleSystem::Start(float secs) {
  running = true;

  if( secs < 0.0001f ) {
    return;
  }

  for( float f = 0; f < secs; f+=1 ) {
    Tick(1);
  }
}

void ParticleSystem::Stop() {
  running = false;

  while( !particles.empty() ) {
    delete particles.back();
    particles.pop_back();
  }
}
