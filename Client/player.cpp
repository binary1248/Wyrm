#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "objects/objects.h"
#include "objectmanager.h"
#include "player.h"

Player* player = 0;

Player::Player(sf::Uint16 id_, sf::String name_) {
  id = id_;
  name = name_;
}

Player::~Player() {

}

void Player::SetShip(sf::Uint16 id_) {
  Object* o = objectmanager.GetObjectById(id_);
  if(!o || (o->type != SHIP)) {
    std::cout << "Could not set player ship." << std::endl;
    return;
  }
  Ship* playerShip = (Ship*)(objectmanager.GetObjectById(id_));
  playerShip->isPlayer = true;
}
