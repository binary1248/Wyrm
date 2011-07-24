#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include <map>
#include <SFML/Network.hpp>

class Game;

class NetworkManager {
  public:
    NetworkManager();
    ~NetworkManager();

    void Tick(float time);

    void ReceiveData(Player*, sf::TcpSocket*);
    void SendData(Player*, sf::Packet&);

    bool IsListening();

    void AddPlayer(Player* p, sf::TcpSocket* s);
    void RemovePlayer(Player* p);
  private:
    bool listening;
    void HandlePacket(sf::Packet p, sf::Uint16 id);
    void AcceptSocket();

    sf::TcpListener sock_listener;
    sf::SocketSelector selector;

    std::map<Player*, sf::TcpSocket*> players;
};

#define PROTOCOL_VER_MAJOR 0.1f
#define PROTOCOL_VER_MINOR 0.5f

enum server_packet_t0{
  OBJECT = 0,
  SET_ID
};

enum client_packet_t0{
  COMMAND = 0
};

enum server_packet_t1{
  UPDATE = 0,
  STATE,
  REMOVE
};

enum client_packet_t1{
  CONTROL = 0
};

#endif // NETWORK_H_INCLUDED
