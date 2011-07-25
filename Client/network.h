#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include <SFML/Network.hpp>

#define PROTOCOL_VER_MAJOR 0.1f
#define PROTOCOL_VER_MINOR 0.6f

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

class NetworkHandler {
  public:
    NetworkHandler();
    ~NetworkHandler();

    int Connect(sf::String username, sf::String password);
    void Disconnect();

    void Send(sf::Packet p);

    void HandlePacket(sf::Packet p);

    void Tick();

    bool IsAuthenticated();
    bool IsConnected();
  private:
    sf::TcpSocket Client;
    sf::SocketSelector Selector;

    bool connected;
    bool authenticated;
};

#endif // NETWORK_H_INCLUDED
