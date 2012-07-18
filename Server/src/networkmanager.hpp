#ifndef NETWORK_HPP_INCLUDED
#define NETWORK_HPP_INCLUDED

#include <map>
#include <memory>

#include <SFML/Network.hpp>

class Game;
class Player;

typedef std::shared_ptr<Player> PlayerPtr;
typedef std::weak_ptr<Player> PlayerWeakPtr;

typedef std::shared_ptr<sf::TcpSocket> SocketPtr;
typedef std::weak_ptr<sf::TcpSocket> SocketWeakPtr;
typedef std::shared_ptr<sf::Packet> PacketPtr;

class NetworkManager {
	public:
		NetworkManager();
		~NetworkManager();

		void Tick( float time );

		void SendData( PlayerPtr, PacketPtr );

		bool IsListening() const;

	private:
		typedef std::map<PlayerWeakPtr, SocketPtr, std::owner_less<PlayerWeakPtr> > PlayerMap;

		std::string ErrCode( sf::Socket::Status status );

		void ReceiveData( PlayerPtr, SocketPtr );
		void HandlePacket( sf::Packet player, sf::Uint16 id );
		void AcceptSocket();
		void AddPlayer( PlayerPtr player, SocketPtr socket );

		bool m_listening;

		sf::TcpListener m_sock_listener;
		sf::SocketSelector m_selector;

		PlayerMap m_players;
};

typedef std::shared_ptr<NetworkManager> NetworkManagerPtr;

#define PROTOCOL_VER_MAJOR 0.1f
#define PROTOCOL_VER_MINOR 0.9f

enum ServerToClient {
	SERVER_OBJECT = 0,
	SERVER_SET_ID,
	SERVER_INVENTORY,
	SERVER_RESOURCE,
	SERVER_SYSTEM
};

enum ClientToServer {
	CLIENT_COMMAND = 0
};

#endif // NETWORK_HPP_INCLUDED
