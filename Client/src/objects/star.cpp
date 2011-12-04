#include <SFML/Graphics.hpp>

#include <config.hpp>
#include <game.hpp>
#include <network.hpp>
#include <objectmanager.hpp>
#include <particlesystem/particlesystem.hpp>
#include <objects/objects.hpp>
#include <utility.hpp>
#include <objects/star.hpp>

REGISTER_FACTORY( STAR, Star );

Star::Star( sf::Uint16 id, sf::String name, sf::Vector2f position,
            sf::Vector2f velocity, float rotation, float rotational_velocity ) :
	Object( STAR, id, name, position, velocity, rotation, rotational_velocity ),
	m_angle( 0.0f ) {
  CreateParticleSystem();
}

Star::Star( sf::Uint16 id, sf::Packet& packet ) :
	Object( STAR, id, packet ) {
  packet >> m_angle >> m_anchor.x >> m_anchor.y;

  CreateParticleSystem();
}

Star::~Star() {
}

void Star::CreateParticleSystem() {
  m_particle_system = std::make_shared<ParticleSystem>();

  ParticlePtr particle = std::make_shared<Particle>( Game::GetGame()->GetResourceManager()->GetParticle( 64, 255.0f, 1.8f ) );
  particle->SetVelocity( sf::Vector2f( 8.0f, 0.0f ) );
  particle->SetColorStart( sf::Color( 255, 255, 220, 255 ) );
  particle->SetColorEnd( sf::Color( 255, 200, 0, 0 ) );
  particle->SetLifetime( 10.0f );
  particle->SetSizeStart( sf::Vector2f( 200.0f, 200.0f ) );
  particle->SetSizeEnd( sf::Vector2f( 200.0f, 200.0f ) );

  ParticleEmitterPtr emitter = std::make_shared<ParticleEmitter>();
  emitter->SetPrototype( particle );
  emitter->SetRate( 5.0f );
  emitter->SetSpread( 180.0f );

  m_particle_system->SetPosition( GetPosition() );
  m_particle_system->AddEmitter( emitter );
  m_particle_system->Start( 12.0f );
}

void Star::Update( float time ) {
  m_angle += GetVelocity().x * time;

  while( m_angle > 360 ) {
    m_angle -= 360;
  }
  while( m_angle < (-360) ) {
    m_angle += 360;
  }

  Object::Update( time );

	sf::Vector2f position;

  position.x = static_cast<float>( cos( clean_angle( m_angle ) ) ) * GetVelocity().y;
  position.y = static_cast<float>( sin( clean_angle( m_angle ) ) ) * GetVelocity().y;

  position += m_anchor;
  SetPosition( position );

  m_particle_system->SetPosition( position );

  m_particle_system->Tick( time );
}

void Star::Draw( sf::RenderWindow& target ) {
  m_particle_system->Draw( target );
}

void Star::HandlePacket( sf::Packet& packet ) {
  sf::Uint16 type1;
  packet >> type1;

  switch(type1) {
    case OBJECT_UPDATE: {
      Object::HandlePacket( packet );
      packet >> m_angle >> m_anchor.x >> m_anchor.y;
      break;
    }
    case OBJECT_STATE: {
      sf::Uint16 type;
      sf::String name;
      packet >> type >> name;
      assert( type == GetType() );
      SetName( name );
      Object::HandlePacket( packet );
			packet >> m_angle >> m_anchor.x >> m_anchor.y;
      break;
    }
    case OBJECT_REMOVE: {
      Delete();
      break;
    }
  }
}
