#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <vector>
#include <SFML/Network.hpp>

class Player {
  public:
    Player(sf::Uint16, sf::String n, sf::TcpSocket* s);
    ~Player();

    void SetShip(sf::Uint16 id);

    // Network Handlers
    void HandlePacket(sf::Packet p);
    void SendPacket(sf::Packet p);
    void Disconnect();

    sf::String name;
    sf::Uint16 id;
    sf::Uint16 shipId;
    sf::TcpSocket* connection;
};

#endif // PLAYER_H_INCLUDED
