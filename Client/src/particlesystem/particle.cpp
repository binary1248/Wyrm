#include <game.hpp>
#include <resourcemanager.hpp>
#include <particlesystem/particle.hpp>

Particle::Particle( TexturePtr texture ) :
	m_texture( texture ),
	m_life( 0.0f ),
	m_lifetime( 1.0f ),
	m_visible( true ) {
  m_sprite.SetTexture( *m_texture );
}

Particle::Particle( const ParticlePtr& particle ) :
	m_life( 0.0f ) {
	SetLifetime( particle->m_lifetime );

  SetSizeStart( particle->m_size_start );
  SetSizeEnd( particle->m_size_end );

  SetColorStart( particle->m_color_start );
  SetColorEnd( particle->m_color_end );

  SetPosition( particle->m_position );
  SetVelocity( particle->m_velocity );
  SetAcceleration( particle->m_acceleration );

  SetVisible( particle->m_visible );

  SetTexture( particle->m_texture );

  m_sprite.SetOrigin( static_cast<float>( m_texture->GetWidth() ) / 2.f, static_cast<float>( m_texture->GetHeight() ) / 2.f );
}

Particle& Particle::operator=( const Particle& particle ) {
	m_life = 0.0f;
	SetLifetime( particle.m_lifetime );

  SetSizeStart( particle.m_size_start );
  SetSizeEnd( particle.m_size_end );

  SetColorStart( particle.m_color_start );
  SetColorEnd( particle.m_color_end );

  SetPosition( particle.m_position );
  SetVelocity( particle.m_velocity );
  SetAcceleration( particle.m_acceleration );

  SetVisible( particle.m_visible );

  SetTexture( particle.m_texture );

  m_sprite.SetOrigin( static_cast<float>( m_texture->GetWidth() ) / 2.f, static_cast<float>( m_texture->GetHeight() ) / 2.f );

  return *this;
}

Particle::~Particle() {

}

float Particle::GetLifetime() const {
	return m_lifetime;
}

void Particle::SetLifetime( float lifetime ) {
	m_lifetime = lifetime;
	m_lifetime = m_lifetime > 0.f ? m_lifetime : 1.f;
	m_size_step = ( m_size_end - m_size_start ) / m_lifetime;
}

void Particle::SetSizeStart( const sf::Vector2f& size ) {
	m_size_start = size;
	m_size_step = ( m_size_end - m_size_start ) / m_lifetime;
}

void Particle::SetSizeEnd( const sf::Vector2f& size ) {
	m_size_end = size;
	m_size_step = ( m_size_end - m_size_start ) / m_lifetime;
}

void Particle::SetColorStart( const sf::Color& color ) {
	m_color_start = color;
}

void Particle::SetColorEnd( const sf::Color& color ) {
	m_color_end = color;
}

void Particle::SetTexture( const TexturePtr& texture ) {
  m_texture = texture;
  m_sprite.SetTexture( *m_texture );
}

void Particle::Tick( float time ) {
  m_life += time;

  if( !IsAlive() ) {
    return;
  }

  m_velocity += ( m_acceleration * time );
  m_sprite.Move( m_velocity * time );

  float factor = m_life / ( m_lifetime > 0.f ? m_lifetime : 1.f );

  sf::Uint8 factor_start = static_cast<unsigned char>( ( 1 - factor ) * 255.f );
  sf::Uint8 factor_end = static_cast<unsigned char>( factor * 255.f );

  m_color = m_color_start * sf::Color( factor_start, factor_start, factor_start, factor_start );
  m_color += m_color_end * sf::Color( factor_end, factor_end, factor_end, factor_end );

  m_sprite.SetColor( m_color );
  m_sprite.Resize( m_size_start + m_life * m_size_step );
}

void Particle::Draw( sf::RenderTarget& target )
{
  if( IsAlive() && m_visible ) {
    target.Draw( m_sprite );
  }
}

void Particle::SetVisible( bool visible ) {
	m_visible = visible;
}

bool Particle::IsAlive() const {
	return m_life <= m_lifetime;
}

void Particle::SetPosition( const sf::Vector2f& position ) {
	m_position = position;
}

const sf::Vector2f& Particle::GetVelocity() const {
	return m_velocity;
}

void Particle::SetVelocity( const sf::Vector2f& velocity ) {
	m_velocity = velocity;
}

const sf::Vector2f& Particle::GetAcceleration() const {
	return m_acceleration;
}

void Particle::SetAcceleration( const sf::Vector2f& acceleration ) {
	m_acceleration = acceleration;
}
