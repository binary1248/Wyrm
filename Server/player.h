#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <list>
#include <set>
#include <SFML/Network.hpp>
#include "objects/object.h"

class Player {
  public:
    Player(sf::TcpSocket* s, Object* o);
    ~Player();

    // Network Handlers
    void Update();
    void SendPacket(sf::Packet& p, bool prio = false );
    void ReceivePacket(sf::Packet*);
    void HandlePacket(sf::Packet& p);
    void Auth(sf::Packet p);
    void Kill();
    inline bool Alive() { return alive; }
    inline std::string GetIPAddressString() { return connection->GetRemoteAddress().ToString(); }
    inline sf::TcpSocket* GetConnection() { return connection; }

    // ID Handlers
    inline sf::Uint16 GetId() { return id; }
    inline Object* GetAgent() { return agent; }
    void SetAgent(Object* o);

    // View Handlers
    void AddObjectToView(Object* o);
    void RemoveObjectFromView(Object* o);
  private:
    void Send(sf::Packet& p);
    void Receive();
    void FlushBuffer();

    // ID vars
    sf::Uint16 id;
    sf::String name;
    Object* agent;

    // Network vars
    sf::TcpSocket* connection;
    bool alive;
    bool half_open;
    std::list<sf::Packet*> send_buffer;
    std::list<sf::Packet*> recv_buffer;

    // Player view vars
    std::vector<Object*> view;
};

#endif // PLAYER_H_INCLUDED
