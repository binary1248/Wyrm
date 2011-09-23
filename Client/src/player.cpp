#include <iostream>
#include <cmath>

#include <boost/make_shared.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "items/item.h"
#include "objects/objects.h"
#include "objectmanager.h"
#include "player.h"
#include "game.h"

Player::Player(sf::Uint16 id_, sf::String name_) {
  id = id_;
  name = name_;

  tentativeAgentId = 0xffff;

  inventory = boost::make_shared<Inventory>();
}

Player::~Player() {
}

void Player::Tick(float time) {
  if( tentativeAgentId != 0xffff ) {
    SetShip(tentativeAgentId);
  }
}

void Player::SetShip(sf::Uint16 id_) {
  tentativeAgentId = id_;
  ObjectPtr o = Game::GetGame()->GetObjectManager()->GetObjectById(id_);

  if( !o ) {
    return;
  }

  if( o->type != SHIP ) {
    return;
  }

  ShipPtr playerShip = boost::static_pointer_cast<Ship, Object>( Game::GetGame()->GetObjectManager()->GetObjectById( id_ ) );
  playerShip->SetPlayer(true);
  std::cout << "Set player ship to object: " << id_ << std::endl;
  tentativeAgentId = 0xffff;
}
