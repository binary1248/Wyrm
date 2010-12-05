#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <SFML/System.hpp>

class Player{
  public:
    Player(sf::Uint16 id_, sf::String name_);
    ~Player();

    void SetShip(sf::Uint16 id_);

    sf::Uint16 id;
    sf::String name;
};


extern Player* player;

#endif // PLAYER_H_INCLUDED
