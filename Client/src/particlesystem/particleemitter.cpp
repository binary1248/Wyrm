#include <cmath>

#include <particlesystem/particle.hpp>
#include <particlesystem/particlesystem.hpp>
#include <particlesystem/particleemitter.hpp>

std::mt19937 ParticleEmitter::rng;

ParticleEmitter::ParticleEmitter() :
  m_spread( 0.0f ),
  m_radius_start( 0.0f ),
  m_radius_end( 0.0f ),
  m_rate( 0.0f ),
  m_queue( 0.0f ) {

}

ParticleEmitter::~ParticleEmitter() {
}

void ParticleEmitter::Tick( const ParticleSystemPtr& particle_system, float time ) {
  m_queue += m_rate * time;

	std::deque<ParticlePtr> particle_pool = particle_system->GetPool();

  while( m_queue >= 1.0f ) {
		ParticlePtr particle;

  	if( !particle_pool.empty() ) {
			particle = particle_pool.back();
			particle_pool.pop_back();
			( *particle ) = ( *m_prototype );
  	}

		if( !particle ) {
			particle = std::make_shared<Particle>( m_prototype );
		}

		std::uniform_real_distribution<float> u1( -m_spread, m_spread );

    float angle = u1( rng );

    float start_dist = 0.0f;

    if( m_radius_end - m_radius_start ) {
    	std::uniform_real_distribution<float> u2( m_radius_start, m_radius_end );

      start_dist = u2( rng );
    }

    sf::Vector2f acceleration = particle->GetAcceleration();
    sf::Vector2f velocity = particle->GetVelocity();

    sf::Vector2f position = sf::Vector2f( start_dist * static_cast<float>( cos( angle ) ), start_dist * static_cast<float>( sin( angle ) ) );

    velocity = sf::Vector2f( velocity.x * static_cast<float>( cos( angle ) ) -
                             velocity.y * static_cast<float>( sin( angle ) ) ,
                             velocity.x * static_cast<float>( sin( angle ) ) +
                             velocity.y * static_cast<float>( cos( angle ) ) );

    acceleration = sf::Vector2f( acceleration.x * static_cast<float>( cos( angle ) ) -
                                 acceleration.y * static_cast<float>( sin( angle ) ) ,
                                 acceleration.x * static_cast<float>( sin( angle ) ) +
                                 acceleration.y * static_cast<float>( cos( angle ) ) );

    particle->SetPosition( position + particle_system->GetPosition() );
    particle->SetAcceleration( acceleration );
    particle->SetVelocity( velocity );

    particle_system->AddParticle( particle );
    m_queue -= 1.0f;
  }
}

void ParticleEmitter::SetPrototype( const ParticlePtr& particle ) {
	m_prototype = particle;
}

void ParticleEmitter::SetSpread( float spread ) {
	m_spread = spread;
}

void ParticleEmitter::SetRadiusStart( float radius ) {
	m_radius_start = radius;
}

void ParticleEmitter::SetRadiusEnd( float radius ) {
	m_radius_end = radius;
}

void ParticleEmitter::SetRate( float rate ) {
	m_rate = rate;
}
