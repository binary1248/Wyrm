#ifndef PLAYERMANAGER_H_INCLUDED
#define PLAYERMANAGER_H_INCLUDED

#include <vector>
#include <SFML/Network.hpp>

#include "player.h"

class PlayerManager {
  public:
    PlayerManager();
    ~PlayerManager();

    sf::Uint16 CreatePlayer(sf::String name, sf::TcpSocket* sock);
    void RemovePlayer(sf::Uint16 id);
    void ClearPlayers();

    void SendToPlayerById(sf::Uint16 id, sf::Packet p);
    void DispatchPacket(sf::Packet p, sf::Uint16 id);

    void Broadcast(sf::Packet p);

  private:
    std::vector<Player*> players;
    std::vector<Player*>::iterator iter;
    sf::Uint16 lastId;
};

#endif // PLAYERMANAGER_H_INCLUDED
