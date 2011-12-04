#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <config.hpp>
#include <items/item.hpp>
#include <objects/objects.hpp>
#include <objectmanager.hpp>
#include <player.hpp>
#include <game.hpp>

Player::Player( sf::Uint16 id, sf::String name ) :
	m_name( name ),
	m_id( id ),
	m_tentative_agent_id( 0xffff ) {
  m_inventory = std::make_shared<Inventory>();
}

Player::~Player() {
}

void Player::Tick( float /*time*/ ) {
  if( m_tentative_agent_id != 0xffff ) {
    SetShip( m_tentative_agent_id );
  }
}

void Player::SetShip( sf::Uint16 id ) {
  m_tentative_agent_id = id;
  ObjectPtr object = Game::GetGame()->GetObjectManager()->GetObjectById( id );

  if( !object ) {
    return;
  }

  if( object->GetType() != SHIP ) {
    return;
  }

  ShipPtr playerShip = std::static_pointer_cast<Ship>( Game::GetGame()->GetObjectManager()->GetObjectById( id ) );
  playerShip->SetPlayer( true );
  std::cout << "Set player ship to object: " << id << "\n";
  m_tentative_agent_id = 0xffff;
}

const InventoryPtr& Player::GetInventory() const {
	return m_inventory;
}
