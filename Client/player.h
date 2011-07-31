#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <SFML/System.hpp>
#include "inventory.h"

class Player{
  public:
    Player(sf::Uint16 id_, sf::String name_);
    ~Player();

    void Tick(float time);

    void SetShip(sf::Uint16 id_);

    inline Inventory* GetInventory() { return inventory; }

    sf::Uint16 id;
    sf::String name;
  private:
    sf::Uint16 tentativeAgentId;

    Inventory* inventory;
};

#endif // PLAYER_H_INCLUDED
