#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include <map>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <SFML/Network.hpp>

class Game;
class Player;

typedef boost::shared_ptr<Player> PlayerPtr;
typedef boost::weak_ptr<Player> PlayerWeakPtr;

typedef boost::shared_ptr<sf::TcpSocket> SocketPtr;
typedef boost::weak_ptr<sf::TcpSocket> SocketWeakPtr;
typedef boost::shared_ptr<sf::Packet> PacketPtr;

std::string ErrCode( sf::Socket::Status status );

class NetworkManager {
  public:
    NetworkManager();
    ~NetworkManager();

    void Tick( float time );

    void SendData( PlayerPtr, PacketPtr );

    bool IsListening() const;

  private:
		void ReceiveData( PlayerPtr, SocketPtr );
    void HandlePacket( sf::Packet player, sf::Uint16 id );
    void AcceptSocket();
    void AddPlayer( PlayerPtr player, SocketPtr socket );

    bool m_listening;

    sf::TcpListener m_sock_listener;
    sf::SocketSelector m_selector;

    std::map<PlayerWeakPtr, SocketPtr> m_players;
};

typedef boost::shared_ptr<NetworkManager> NetworkManagerPtr;

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
