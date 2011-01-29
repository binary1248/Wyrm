#ifndef PLAYERMANAGER_H_INCLUDED
#define PLAYERMANAGER_H_INCLUDED

#include <vector>
#include <SFML/Network.hpp>
#include "player.h"

class Game;

class PlayerManager {
  public:
    PlayerManager();
    ~PlayerManager();

    void Tick(float time);

    sf::Uint16 CreatePlayer(sf::TcpSocket* sock);
    void RemovePlayer(sf::Uint16 id);
    void ClearPlayers();

    void SendToPlayerById(sf::Uint16 id, sf::Packet p);

    void Broadcast(sf::Packet p);

  private:
    std::vector<Player*> players;
    sf::Uint16 lastId;
};

#endif // PLAYERMANAGER_H_INCLUDED
