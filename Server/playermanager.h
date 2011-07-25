#ifndef PLAYERMANAGER_H_INCLUDED
#define PLAYERMANAGER_H_INCLUDED

#include <vector>
#include <SFML/Network.hpp>
#include "player.h"

class Game;
class Object;

class PlayerManager {
  public:
    PlayerManager();
    ~PlayerManager();

    void Tick(float time);

    Player* CreatePlayer();
    void RemovePlayer(Player* p);
    void ClearPlayers();

    void BroadcastNewObject(Object* o);
    void Broadcast(sf::Packet p);

    inline sf::Uint16 NewID() { return lastId++; }
  private:
    std::vector<Player*> players;
    sf::Uint16 lastId;
};

#endif // PLAYERMANAGER_H_INCLUDED
