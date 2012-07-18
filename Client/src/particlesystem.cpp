#include <GL/glew.h>

#include <config.hpp>
#include <utility.hpp>
#include <game.hpp>
#include <particlesystem.hpp>

std::mt19937 ParticleSystem::rng;

ParticleSystem::ParticleSystem() :
	m_running( false ),
	m_emitting( false ) {
	glGenBuffers( 1, &m_particles_vbo );
}

ParticleSystem::~ParticleSystem() {
	glDeleteBuffers( 1, &m_particles_vbo );

	Stop();
}

ParticleSystem::ParticleSystem( const ParticleSystem& particle_system ) {
	m_position = particle_system.m_position;
	m_emitters = particle_system.m_emitters;

	m_particles.clear();
	m_particle_vbo_data.clear();

	m_particles_vbo = 0;

	m_texture = TexturePtr( particle_system.m_texture );

	m_running = false;
	m_emitting = false;

	glGenBuffers( 1, &m_particles_vbo );
}

void ParticleSystem::Draw( sf::RenderTarget& /*target*/ ) {
	std::size_t particles_size = m_particles.size();

	if( !particles_size || m_emitters.empty() ) {
		return;
	}

	sf::Vector2f position;
	sf::Vector2f size;
	Particle::Color color;

	// sf::Vector2f view_position = target.getView().getCenter() - target.getView().getSize() / 2.f;

	m_particle_vbo_data.reserve( particles_size );

	for( std::size_t index = 0; index < particles_size; ++index ) {
		float factor = m_particles[ index ].life / m_particles[ index ].lifetime;

		position = m_particles[ index ].position;

		size = m_particles[ index ].size_start * ( 1.0f - factor ) + m_particles[ index ].size_end * factor;

		color.r = m_particles[ index ].color_start.r * ( 1.0f - factor ) + m_particles[ index ].color_end.r * factor;
		color.g = m_particles[ index ].color_start.g * ( 1.0f - factor ) + m_particles[ index ].color_end.g * factor;
		color.b = m_particles[ index ].color_start.b * ( 1.0f - factor ) + m_particles[ index ].color_end.b * factor;
		color.a = m_particles[ index ].color_start.a * ( 1.0f - factor ) + m_particles[ index ].color_end.a * factor;

		ParticleVBOData particle;

		// Color data
		particle.r0 = particle.r1 = particle.r2 = particle.r3 = color.r;
		particle.g0 = particle.g1 = particle.g2 = particle.g3 = color.g;
		particle.b0 = particle.b1 = particle.b2 = particle.b3 = color.b;
		particle.a0 = particle.a1 = particle.a2 = particle.a3 = color.a;

		// Texture data
		particle.u0 = 0.0f; particle.v0 = 0.0f;
		particle.u1 = 0.0f; particle.v1 = 1.0f;
		particle.u2 = 1.0f; particle.v2 = 1.0f;
		particle.u3 = 1.0f; particle.v3 = 0.0f;

		// Vertex data
		particle.x0 = position.x - size.x / 2.0f;
		particle.y0 = position.y - size.y / 2.0f;
		particle.x1 = position.x - size.x / 2.0f;
		particle.y1 = position.y + size.y / 2.0f;
		particle.x2 = position.x + size.x / 2.0f;
		particle.y2 = position.y + size.y / 2.0f;
		particle.x3 = position.x + size.x / 2.0f;
		particle.y3 = position.y - size.y / 2.0f;

		m_particle_vbo_data.push_back( particle );
	}

	m_texture->bind( sf::Texture::Normalized );

	glBindBuffer( GL_ARRAY_BUFFER, m_particles_vbo );
	glBufferData( GL_ARRAY_BUFFER, 0, 0, GL_STREAM_DRAW );
	glBufferData( GL_ARRAY_BUFFER, particles_size * 4 * 8 * sizeof( GLfloat ), &m_particle_vbo_data[0], GL_STREAM_DRAW );

	glColorPointer( 4, GL_FLOAT, 8 * sizeof( GLfloat ), 0 );
	glTexCoordPointer( 2, GL_FLOAT, 8 * sizeof( GLfloat ), reinterpret_cast<GLvoid*>( 4 * sizeof( GLfloat ) ) );
	glVertexPointer( 2, GL_FLOAT, 8 * sizeof( GLfloat ), reinterpret_cast<GLvoid*>( 6 * sizeof( GLfloat ) ) );

	glEnableClientState( GL_COLOR_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glEnableClientState( GL_VERTEX_ARRAY );

	glDrawArrays( GL_QUADS, 0, particles_size * 4 );

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );

	m_particle_vbo_data.clear();
}

void ParticleSystem::Tick( float time ) {
	if( !m_running ) {
		return;
	}

	std::size_t particles_size = m_particles.size();

	for( std::size_t index = 0; index < particles_size; ) {
		m_particles[ index ].life += time;

		if( m_particles[ index ].life >= m_particles[ index ].lifetime ) {
			m_particles.erase( m_particles.begin() );
			particles_size--;
			continue;
		}

		m_particles[ index ].velocity += ( m_particles[ index ].acceleration * time );
		m_particles[ index ].position += ( m_particles[ index ].velocity * time );

		++index;
	}

	if( !m_emitting ) {
		return;
	}

	std::size_t emitters_size = m_emitters.size();

	for( std::size_t index = 0; index < emitters_size; ++index ) {
		m_emitters[ index ].queue += m_emitters[ index ].rate * time;

		while( m_emitters[ index ].queue >= 1.0f ) {
			Particle particle = m_emitters[ index ].prototype;

			particle.life = 0.0f;
			particle.lifetime = m_emitters[ index ].prototype.lifetime;

			particle.size_start = m_emitters[ index ].prototype.size_start;
			particle.size_end = m_emitters[ index ].prototype.size_end;

			particle.color_start.r = m_emitters[ index ].prototype.color_start.r;
			particle.color_start.g = m_emitters[ index ].prototype.color_start.g;
			particle.color_start.b = m_emitters[ index ].prototype.color_start.b;
			particle.color_start.a = m_emitters[ index ].prototype.color_start.a;

			particle.color_end.r = m_emitters[ index ].prototype.color_end.r;
			particle.color_end.g = m_emitters[ index ].prototype.color_end.g;
			particle.color_end.b = m_emitters[ index ].prototype.color_end.b;
			particle.color_end.a = m_emitters[ index ].prototype.color_end.a;

			std::uniform_real_distribution<float> u1( -m_emitters[ index ].spread, m_emitters[ index ].spread );

			float angle = u1( rng );

			float start_dist = 0.0f;

			std::uniform_real_distribution<float> u2( m_emitters[ index ].radius_start, m_emitters[ index ].radius_end );

			start_dist = u2( rng );

			sf::Vector2f acceleration = particle.acceleration;
			sf::Vector2f velocity = particle.velocity;

			sf::Vector2f position = sf::Vector2f( start_dist * static_cast<float>( cos( angle ) ), start_dist * static_cast<float>( sin( angle ) ) );

			velocity = sf::Vector2f( velocity.x * static_cast<float>( cos( angle ) ) -
															 velocity.y * static_cast<float>( sin( angle ) ) ,
															 velocity.x * static_cast<float>( sin( angle ) ) +
															 velocity.y * static_cast<float>( cos( angle ) ) );

			acceleration = sf::Vector2f( acceleration.x * static_cast<float>( cos( angle ) ) -
																	 acceleration.y * static_cast<float>( sin( angle ) ) ,
																	 acceleration.x * static_cast<float>( sin( angle ) ) +
																	 acceleration.y * static_cast<float>( cos( angle ) ) );

			particle.position = position;
			particle.acceleration = acceleration;
			particle.velocity = velocity;

			m_particles.push_back( particle );
			particles_size++;

			m_emitters[ index ].queue -= 1.0f;
		}
	}
}

void ParticleSystem::Start( float time ) {
	m_running = true;
	m_emitting = true;

	if( time <= 0.0f ) {
		return;
	}

	for( float f = 0.0f; f < time; f += 0.1f ) {
		Tick( 0.1f );
	}
}

void ParticleSystem::Stop() {
	m_running = false;
	m_emitting = false;

	m_particles.clear();
}

void ParticleSystem::StartEmitting() {
	m_emitting = true;
}

void ParticleSystem::StopEmitting() {
	m_emitting = false;
}

void ParticleSystem::AddEmitter( ParticleEmitter emitter ) {
	emitter.queue = 0.0f;
	m_emitters.push_back( emitter );
}

const std::vector<ParticleSystem::ParticleEmitter>& ParticleSystem::GetEmitters() const {
	return m_emitters;
}

const sf::Vector2f& ParticleSystem::GetPosition() const {
	return m_position;
}

void ParticleSystem::SetPosition( const sf::Vector2f& position ) {
	m_position = position;
}

void ParticleSystem::SetTextureParameters( int size, float sigma2, float exp ) {
	m_texture = Game::GetGame()->GetResourceManager()->GetParticle( size, sigma2, exp );

	m_texture->copyToImage().saveToFile("bla.png");
}

ParticleSystem::Particle::Particle() :
	life( 0.0f ),
	lifetime( 0.01f ),
	position( sf::Vector2f( 0.0f, 0.0f ) ),
	velocity( sf::Vector2f( 0.0f, 0.0f ) ),
	acceleration( sf::Vector2f( 0.0f, 0.0f ) ),
	size_start( sf::Vector2f( 10.0f, 10.0f ) ),
	size_end( sf::Vector2f( 10.0f, 10.0f ) ),
	color_start( { 1.0f, 1.0f, 1.0f, 1.0f } ),
	color_end( { 1.0f, 1.0f, 1.0f, 1.0f } )
{
}
