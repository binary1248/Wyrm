#include <cmath>

#include "particle.h"
#include "particlesystem.h"
#include "particleemitter.h"

ParticleEmitter::ParticleEmitter(ParticleSystem* ps) : system(ps) {
  prototype = 0;
  queue = 0;
  spread = 0;
  radius_start = 0;
  radius_end = 0;
  rate = 0;
}

ParticleEmitter::~ParticleEmitter() {
  if( prototype ) {
    delete prototype;
  }
}

void ParticleEmitter::Tick(float secs) {
  //float rate_per_ms = rate / 1000.0f;

  queue += rate * secs;

  while( queue >= 1.0f ) {
    Particle* p = new Particle(*prototype);

    float angle = static_cast<float>(rand()%(static_cast<int>(spread)*2)) - spread;

    float start_dist = 0;

    if( radius_end-radius_start ) {
      start_dist = static_cast<float>(rand()%(static_cast<int>(std::abs(radius_end-radius_start))*2)) - radius_start;
    }

    sf::Vector2f acceleration = p->GetAcceleration();
    sf::Vector2f velocity = p->GetVelocity();

    sf::Vector2f position = sf::Vector2f( start_dist * cos(angle), start_dist * sin(angle) );

    velocity = sf::Vector2f( velocity.x * cos(angle)-
                             velocity.y * sin(angle),
                             velocity.x * sin(angle)+
                             velocity.y * cos(angle) );

    acceleration = sf::Vector2f( acceleration.x * cos(angle)-
                                 acceleration.y * sin(angle),
                                 acceleration.x * sin(angle)+
                                 acceleration.y * cos(angle) );

    p->SetPosition(position + system->GetPosition());
    p->SetAcceleration(acceleration);
    p->SetVelocity(velocity);

    system->AddParticle(p);
    queue -= 1.0f;
  }
}
