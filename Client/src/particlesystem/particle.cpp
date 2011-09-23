#include "../game.h"
#include "../resourcemanager.h"
#include "particle.h"

Particle::Particle(TexturePtr i) {
  alive = true;
  visible = true;
  life = 0.0f;
  lifetime = 1.0f;
  texture = i;

  sprite.SetTexture(*texture);
}

Particle::Particle( const ParticlePtr p ) {
  alive = true;
  life = 0.0f;

  visible = p->visible;
  lifetime = p->lifetime;
  size_start = p->size_start;
  size_end = p->size_end;
  color_start = p->color_start;
  color_end = p->color_end;
  position = p->position;
  velocity = p->velocity;
  acceleration = p->acceleration;

  texture = p->texture;

  sprite.SetTexture(*texture);
  sprite.SetOrigin(texture->GetWidth()/2,texture->GetHeight()/2);
}

Particle::~Particle() {

}

void Particle::SetLifetime(float l) {
	lifetime = l;
	lifetime = lifetime > 0.f ? lifetime : 1.f;
	size_step = ( size_end - size_start ) / lifetime;
}

void Particle::SetSizeStart(sf::Vector2f s) {
	size_start = s;
	size_step = ( size_end - size_start ) / lifetime;
}

void Particle::SetSizeEnd(sf::Vector2f s) {
	size_end = s;
	size_step = ( size_end - size_start ) / lifetime;
}

void Particle::SetColorStart(sf::Color c) {
	color_start = c;
}

void Particle::SetColorEnd(sf::Color c) {
	color_end = c;
}

void Particle::SetTexture(TexturePtr i) {
  texture = i;
  sprite.SetTexture(*texture);
}

void Particle::Tick(float secs) {
  life += secs;

  if( life > lifetime ) {
    Kill();
    return;
  }

  velocity += (acceleration * secs);
  sprite.Move( velocity * secs );

  float factor = life / ( lifetime > 0.f ? lifetime : 1.f );

  sf::Uint8 factor_start = static_cast<unsigned char>( (1-factor) * 255.f );
  sf::Uint8 factor_end = static_cast<unsigned char>( factor * 255.f );

  color = color_start * sf::Color( factor_start, factor_start, factor_start, factor_start );
  color += color_end * sf::Color( factor_end, factor_end, factor_end, factor_end );

  sprite.SetColor( color );
  sprite.Resize( size_start + life * size_step );
}

void Particle::Draw(sf::RenderTarget& r)
{
  if( alive && visible ) {
    r.Draw(sprite);
  }
}
