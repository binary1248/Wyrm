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

    sf::Uint16 CreatePlayer(sf::TcpSocket* sock);
    void RemovePlayer(sf::Uint16 id);
    void ClearPlayers();

    void SendToPlayerById(sf::Uint16 id, sf::Packet p);

    void BroadcastNewObject(Object* o);

    void Broadcast(sf::Packet p);

    inline sf::Uint16 NewID() { return lastId++; }
  private:
    std::vector<Player*> players;
    sf::Uint16 lastId;
};

#endif // PLAYERMANAGER_H_INCLUDED
