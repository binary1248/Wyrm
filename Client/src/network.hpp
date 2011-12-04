#ifndef NETWORK_HPP_INCLUDED
#define NETWORK_HPP_INCLUDED

#include <memory>

#include <SFML/Network.hpp>

class NetworkHandler {
  public:
    NetworkHandler();
    ~NetworkHandler();

    int Connect( sf::String username, sf::String password );
    void Disconnect();

    void Send( sf::Packet packet );

    void HandlePacket( sf::Packet packet );

    void Tick();

    bool IsAuthenticated();
    bool IsConnected();
  private:
    sf::TcpSocket m_socket;
    sf::SocketSelector m_selector;

    bool m_connected;
    bool m_authenticated;
};

typedef std::shared_ptr<NetworkHandler> NetworkHandlerPtr;

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

#endif // NETWORK_HPP_INCLUDED
