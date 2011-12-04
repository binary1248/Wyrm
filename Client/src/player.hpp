#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include <SFML/System.hpp>

#include <inventory.hpp>

class Player{
  public:
    Player( sf::Uint16 id, sf::String name );
    ~Player();

    void Tick( float time );

    void SetShip( sf::Uint16 id );

    const InventoryPtr& GetInventory() const;

  private:
    InventoryPtr m_inventory;

    sf::String m_name;
    sf::Uint16 m_id;

    sf::Uint16 m_tentative_agent_id;
};

enum packet_client_command {
  COMMAND_CONTROL = 0
};

typedef std::shared_ptr<Player> PlayerPtr;

#endif // PLAYER_HPP_INCLUDED
