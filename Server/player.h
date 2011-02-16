#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <list>
#include <SFML/Network.hpp>
#include "objects/object.h"

class Player {
  public:
    Player(sf::Uint16, sf::TcpSocket* s);
    ~Player();

    // Network Handlers
    void Update();
    void SendPacket(sf::Packet p, bool prio = false );
    void ReceivePacket();

    void HandlePacket(sf::Packet p);

    void Auth(sf::Packet p);
    void Disconnect();

    bool Connected();
    std::string GetIPAddressString();

    sf::Uint16 GetId();

    Object* GetAgent();
    void SetAgent(Object* o);

  private:
    sf::TcpSocket* connection;
    bool connected;
    bool half_open;
    sf::Uint16 id;
    std::list<sf::Packet*> buffer;

    sf::String name;

    Object* agent;

    void Send(sf::Packet p);
};

#endif // PLAYER_H_INCLUDED
