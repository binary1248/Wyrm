#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include <SFML/Network.hpp>

enum server_packet_t0{
  OBJECT = 0,
};

enum client_packet_t0{
  COMMAND = 0
};

enum server_packet_t1{
  POSITION_UPDATE = 0,
  VELOCITY_UPDATE,
  NEW_OBJECT,
  REMOVE_OBJECT,
  SET_ID
};

enum client_packet_t1{
  CONTROL = 0
};

class NetworkHandler : public sf::Thread{
  public:
    NetworkHandler();
    ~NetworkHandler();

    int Connect(sf::String username, sf::String password);
    void Disconnect();

    void Send(sf::Packet p);

    void HandlePacket(sf::Packet p);

    bool connected;
    bool authenticated;
    bool running;
  private:
    virtual void Run();
    sf::TcpSocket Client;
    sf::SocketSelector Selector;
};

extern NetworkHandler networkhandler;

#endif // NETWORK_H_INCLUDED
