#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <boost/shared_ptr.hpp>

#include <SFML/System.hpp>

#include "inventory.h"

class Player{
  public:
    Player(sf::Uint16 id_, sf::String name_);
    ~Player();

    void Tick(float time);

    void SetShip(sf::Uint16 id_);

    inline InventoryPtr GetInventory() { return inventory; }

    sf::Uint16 id;
    sf::String name;
  private:
    sf::Uint16 tentativeAgentId;

    InventoryPtr inventory;
};

enum packet_client_command {
  COMMAND_CONTROL = 0
};

typedef boost::shared_ptr<Player> PlayerPtr;

#endif // PLAYER_H_INCLUDED
