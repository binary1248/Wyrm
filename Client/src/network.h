#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include <boost/shared_ptr.hpp>

#include <SFML/Network.hpp>

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

typedef boost::shared_ptr<NetworkHandler> NetworkHandlerPtr;

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
