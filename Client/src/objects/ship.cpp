#include <SFML/Graphics.hpp>

#include <config.hpp>
#include <objects/ship.hpp>
#include <objects/objects.hpp>
#include <game.hpp>
#include <utility.hpp>
#include <objectmanager.hpp>

REGISTER_FACTORY(SHIP,Ship);

Ship::Ship( sf::Uint16 id, sf::String name, sf::Vector2f position,
            sf::Vector2f velocity, float rotation, float rotational_velocity ) :
	Object( SHIP, id, name, position, velocity, rotation, rotational_velocity ) {
  Init();
}

Ship::Ship( sf::Uint16 id, sf::Packet& packet ) :
	Object( SHIP, id, packet ) {
  packet >> m_thrust;

  Init();
}

Ship::~Ship() {

}

void Ship::Init() {
  m_is_player = false;

  m_acceleration.x = 0;
  m_acceleration.y = 0;

  TexturePtr texture = Game::GetGame()->GetResourceManager()->OpenImage( "spaceship.png" );

  m_sprite.SetTexture( *texture );
  m_sprite.SetOrigin( static_cast<float>( texture->GetWidth() ) / 2.f, static_cast<float>( texture->GetHeight() ) / 2.f );
  m_sprite.SetScale( 1.0f, 1.0f );

  m_text.SetFont( sf::Font::GetDefaultFont() );
  m_text.SetColor( sf::Color( 0, 255, 0 ) );
  m_text.SetScale( 0.4f, 0.4f );

  m_image_height = static_cast<float>( texture->GetHeight() );
}

void Ship::Update( float time ) {
  m_acceleration.x = static_cast<float>( -cos( clean_angle( GetRotation() ) ) ) * ( -m_thrust );
  m_acceleration.y = static_cast<float>(  sin( clean_angle( GetRotation() ) ) ) * ( -m_thrust );
  SetVelocity( GetVelocity() + ( m_acceleration * time ) );

  Object::Update( time );

  m_sprite.SetPosition(  GetPosition() );
  m_sprite.SetRotation( -GetRotation() );

  sf::FloatRect rect = m_text.GetRect();
  m_text.SetPosition( GetPosition() + sf::Vector2f( -rect.Width / 2 , m_image_height / 3 ) );

  if( m_is_player ) {
    Game::GetGame()->GetBackdrop()->Update( GetVelocity(), GetPosition() );
  }
}

void Ship::Draw( sf::RenderWindow& target ) {
	if( m_is_player ) {
    sf::View view( target.GetDefaultView() );
    view.SetCenter( GetPosition() );
    target.SetView( view );
  }

  m_text.SetString( GetName() );
  target.Draw( m_sprite );
  target.Draw( m_text );
}

void Ship::HandlePacket( sf::Packet& packet ) {
  sf::Uint16 type1;
  packet >> type1;

  switch(type1) {
    case OBJECT_UPDATE: {
      Object::HandlePacket( packet );
      packet >> m_thrust;
      break;
    }
    case OBJECT_STATE: {
      sf::Uint16 type;
      sf::String name;
      packet >> type >> name;
      assert( type == GetType() );
      SetName( name );
      Object::HandlePacket( packet );
      packet >> m_thrust;
      break;
    }
    case OBJECT_REMOVE: {
      Delete();
      break;
    }
  }
}

void Ship::SetPlayer( bool is_player ) {
	m_is_player = is_player;
}

bool Ship::IsPlayer() const {
	return m_is_player;
}
