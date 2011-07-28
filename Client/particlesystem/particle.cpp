#include "game.h"
#include "resourcemanager.h"
#include "particle.h"

Particle::Particle(sf::Image* i) {
  alive = true;
  life = 0.0f;
  visible = true;

  image = i;

  sprite.SetImage(*image);
}

Particle::Particle(const Particle& p) {
  alive = true;
  life = 0.0f;

  visible = p.visible;
  lifetime = p.lifetime;
  size_start = p.size_start;
  size_end = p.size_end;
  color_start = p.color_start;
  color_end = p.color_end;
  position = p.position;
  velocity = p.velocity;
  acceleration = p.acceleration;

  image = p.image;

  sprite.SetImage(*image);
  sprite.SetOrigin(image->GetWidth()/2,image->GetHeight()/2);
}

Particle::~Particle() {

}

void Particle::SetImage(sf::Image* i) {
  image = i;
  sprite.SetImage(*image);
}

void Particle::Tick(float secs) {
  life += secs;

  if( life > lifetime ) {
    Kill();
    return;
  }

  velocity += (acceleration * secs);
  position += (velocity * secs);

  sf::Color c;

  float factor = (life+1)/(lifetime+1);

  c.r = static_cast<unsigned char>((1-factor) * static_cast<float>(color_start.r) + factor * static_cast<float>(color_end.r));
  c.g = static_cast<unsigned char>((1-factor) * static_cast<float>(color_start.g) + factor * static_cast<float>(color_end.g));
  c.b = static_cast<unsigned char>((1-factor) * static_cast<float>(color_start.b) + factor * static_cast<float>(color_end.b));
  c.a = static_cast<unsigned char>((1-factor) * static_cast<float>(color_start.a) + factor * static_cast<float>(color_end.a));

  sprite.SetScale( (1-factor) * size_start + factor * size_end );
  sprite.SetColor(c);
  sprite.SetPosition(position);
}

void Particle::Draw(sf::RenderTarget& r)
{
  if( alive && visible ) {
    r.Draw(sprite);
  }
}
