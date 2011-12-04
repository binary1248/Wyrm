#include <config.hpp>
#include <particlesystem/particle.hpp>
#include <particlesystem/particleemitter.hpp>
#include <particlesystem/particlesystem.hpp>

ParticleSystem::ParticleSystem() {
  m_running = false;
}

ParticleSystem::~ParticleSystem() {
  Stop();
}

void ParticleSystem::Draw( sf::RenderTarget& target ) {
	std::deque<ParticlePtr>::const_iterator particle( m_particles.begin() );
	std::deque<ParticlePtr>::const_iterator end( m_particles.end() );

	for( ; particle != end; ++particle ) {
		(*particle)->Draw( target );
	}
}

void ParticleSystem::Tick( float time ) {
  if( !m_running ) {
    return;
  }

  std::deque<ParticleEmitterPtr>::const_iterator emitter_iter( m_emitters.begin() );
	std::deque<ParticleEmitterPtr>::const_iterator emitter_end( m_emitters.end() );

	std::deque<ParticlePtr>::iterator particle_iter( m_particles.begin() );
	std::deque<ParticlePtr>::iterator particle_end( m_particles.end() );

  while( particle_iter != particle_end ) {
    if( !(*particle_iter)->IsAlive() ) {
			m_particle_pool.push_back( (*particle_iter) );
			particle_iter = m_particles.erase( particle_iter );
			continue;
    }

    (*particle_iter)->Tick( time );
    ++particle_iter;
  }

  for( ; emitter_iter != emitter_end; ++emitter_iter ) {
		(*emitter_iter)->Tick( shared_from_this(), time );
	}

	m_particle_pool.clear();
}

void ParticleSystem::Start( float time ) {
  m_running = true;

  if( time <= 0.f ) {
    return;
  }

  for( float f = 0; f < time; f += 0.1f ) {
    Tick( 0.1f );
  }
}

void ParticleSystem::Stop() {
  m_running = false;

  m_particles.clear();
}

void ParticleSystem::AddEmitter( const ParticleEmitterPtr& emitter ) {
	m_emitters.push_back( emitter );
}

void ParticleSystem::AddParticle( const ParticlePtr& particle ) {
	m_particles.push_back( particle );
}

const sf::Vector2f& ParticleSystem::GetPosition() const {
	return m_position;
}

void ParticleSystem::SetPosition( const sf::Vector2f& position ) {
	m_position = position;
}

const std::deque<ParticlePtr>& ParticleSystem::GetPool() const {
	return m_particle_pool;
}
