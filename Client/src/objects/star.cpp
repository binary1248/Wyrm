#include <iostream>

#include <boost/make_shared.hpp>

#include <SFML/Graphics.hpp>

#include "../game.h"
#include "../network.h"
#include "../objectmanager.h"
#include "../particlesystem/particlesystem.h"
#include "objects.h"
#include "star.h"

#define clean_angle(a) (((a+90)/180)*M_PI)

REGISTER_FACTORY( STAR, Star );

Star::Star(sf::Uint16 id_, sf::String name_, sf::Vector2f pos_, sf::Vector2f vel_, float rot_, float rot_vel_) :
Object(STAR, id_, name_, pos_, vel_, rot_, rot_vel_) {
  id = id_;
  name = name_;
  angle = 0;

  CreateParticleSystem();
}

Star::Star( sf::Uint16 id_, sf::Packet p ) :
Object(STAR, id_, "", sf::Vector2f(0,0), sf::Vector2f(0,0), 0, 0) {
  p >> name >> position.x >> position.y >> velocity.x >> velocity.y
    >> rotation >> rotational_velocity >> angle >> anchor.x >> anchor.y;

  CreateParticleSystem();
}

Star::~Star() {
}

void Star::CreateParticleSystem() {
  ps = boost::make_shared<ParticleSystem>();

  ParticlePtr part = boost::make_shared<Particle>( Game::GetGame()->GetResourceManager()->GetParticle(64, 255, 1.8) );
  part->SetVelocity(sf::Vector2f(8,0));
  part->SetColorStart(sf::Color(255,255,220,255));
  part->SetColorEnd(sf::Color(255,200,0,0));
  part->SetLifetime(10);
  part->SetSizeStart(sf::Vector2f(200,200));
  part->SetSizeEnd(sf::Vector2f(200,200));

  ParticleEmitterPtr pe = boost::make_shared<ParticleEmitter>();
  pe->SetPrototype(part);
  pe->SetRate(5);
  pe->SetSpread(180);

  ps->SetPosition(position);
  ps->AddEmitter(pe);
  ps->Start(12);
}

void Star::Update(float time) {
  angle += velocity.x * time;

  while( angle > 360 ) {
    angle -= 360;
  }
  while( angle < (-360) ) {
    angle += 360;
  }

  position.x = cos( clean_angle(angle) ) * velocity.y;
  position.y = sin( clean_angle(angle) ) * velocity.y;

  position += anchor;

  rotation += rotational_velocity * time;

  ps->SetPosition(position);

  ps->Tick(time);
}

void Star::Draw(sf::RenderWindow& w) {
  ps->Draw(w);
}

void Star::HandlePacket(sf::Packet p) {
  sf::Uint16 type1;
  p >> type1;

  switch(type1) {
    case OBJECT_UPDATE:
    {
      sf::Vector2f pos;
      sf::Vector2f vel;
      p >> pos.x >> pos.y >> rotation >> vel.x >> vel.y >> rotational_velocity >> angle >> anchor.x >> anchor.y;
      position = pos;
      velocity = vel;
      break;
    }
    case OBJECT_STATE:
    {
      sf::Uint16 type;
      sf::Vector2f pos;
      sf::Vector2f vel;
      p >> type >> name >> pos.x >> pos.y >> vel.x >> vel.y >> rotation >> rotational_velocity >> angle >> anchor.x >> anchor.y;
      position = pos;
      velocity = vel;
      break;
    }
    case OBJECT_REMOVE:
    {
      Delete();
      break;
    }
  }
}
