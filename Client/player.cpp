#include <iostream>
#include <cmath>
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

  inventory = new Inventory();
}

Player::~Player() {
  if( inventory ) {
    delete inventory;
  }
}

void Player::Tick(float time) {
  if( tentativeAgentId != 0xffff ) {
    SetShip(tentativeAgentId);
  }
}

void Player::SetShip(sf::Uint16 id_) {
  tentativeAgentId = id_;
  Object* o = Game::GetGame()->GetObjectManager()->GetObjectById(id_);
  if( !o ) {
    //std::cout << "Could not set player ship (not found): " << id_ << std::endl;
    return;
  }
  if( o->type != SHIP ) {
    //std::cout << "Could not set player ship (not ship): " << o->type << std::endl;
    return;
  }
  Ship* playerShip = (Ship*)(Game::GetGame()->GetObjectManager()->GetObjectById(id_));
  playerShip->SetPlayer(true);
  std::cout << "Set player ship to object: " << id_ << std::endl;
  tentativeAgentId = 0xffff;
}
