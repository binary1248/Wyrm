#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include <map>
#include <SFML/Network.hpp>

class Game;
class Player;

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
#define PROTOCOL_VER_MINOR 0.8f

enum packet_server {
  SERVER_OBJECT = 0,
  SERVER_SET_ID,
  SERVER_INVENTORY
};

enum packet_client {
  CLIENT_COMMAND = 0
};

#endif // NETWORK_H_INCLUDED
