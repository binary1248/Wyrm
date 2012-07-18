#include <SFML/Network.hpp>

#include <config.hpp>
#include <utility.hpp>
#include <network.hpp>
#include <player.hpp>
#include <objectmanager.hpp>
#include <game.hpp>

NetworkHandler::NetworkHandler() {
	m_connected = false;
	m_authenticated = false;
}

NetworkHandler::~NetworkHandler() {
	Disconnect();
}

int NetworkHandler::Connect( sf::String username, sf::String password, sf::String address ) {
	if ( m_socket.connect( address.toAnsiString(), 1337 ) != sf::Socket::Done ) {
		// Error...
		LogConsole( "Can't connect to server at " + address.toAnsiString() + "." );
		return -1;
	}

	m_connected = true;

	m_selector.add( m_socket );

	{
		sf::Packet packet;
		m_socket.receive( packet );
		sf::String string;
		float version_major;
		float version_minor;
		packet >> string >> version_major >> version_minor;
		LogConsole( string.toAnsiString() + string_cast( version_major ) + " " + string_cast( version_minor ) );
		if( !(string.toAnsiString().compare( "Wyrm protocol version " )) &&
				 ( version_major == PROTOCOL_VER_MAJOR ) &&
				 ( version_minor == PROTOCOL_VER_MINOR ) ) {
			sf::Packet response;
			response << username << password;
			m_socket.send( response );
		} else {
			LogConsole( "Protocol mismatch" );
			Disconnect();
			return -1;
		}
	}

	{
		sf::Packet packet;
		if( m_socket.receive(packet) != sf::TcpSocket::Done ) {
			LogConsole( "Error while receiving." );
			Disconnect();
			return -1;
		}
		sf::String s;
		packet >> s;
		if( !s.toAnsiString().compare("Authentication successful") ) {
			LogConsole( "Authentication successful" );
			m_authenticated = true;
		} else {
			LogConsole( "Authentication failed: " + s.toAnsiString() );
			Disconnect();
			return -1;
		}
	}

	return 0;
}

void NetworkHandler::Disconnect() {
	m_selector.remove( m_socket );
	m_socket.disconnect();
}

void NetworkHandler::Send( sf::Packet packet ) {
	m_socket.send( packet );
}

void NetworkHandler::HandlePacket( sf::Packet packet ) {

	sf::Uint16 type0;
	packet >> type0;

	switch( type0 ) {
		case ServerToClient::SERVER_OBJECT: {
			Game::GetGame()->GetObjectManager()->DispatchPacket( packet );

			break;
		}
		case ServerToClient::SERVER_SET_ID: {
			sf::Uint16 id;
			packet >> id;

			PlayerPtr player = Game::GetGame()->GetPlayer();

			if( !player ) {
				player = Game::GetGame()->CreatePlayer( 0, "" );
			}

			player->SetAgent( id );

			break;
		}
		case ServerToClient::SERVER_INVENTORY: {
			PlayerPtr player = Game::GetGame()->GetPlayer();

			if( player ) {
				player->GetInventory()->HandlePacket( packet );
			}

			break;
		}
		case ServerToClient::SERVER_RESOURCE: {
			ResourceManagerPtr resourcemanager = Game::GetGame()->GetResourceManager();

			if( resourcemanager ) {
				resourcemanager->GenerateResourceFromPacket( packet );
			}

			break;
		}
		case ServerToClient::SERVER_SYSTEM: {
			Game::GetGame()->SetSystem( std::make_shared<System>( Game::GetGame()->GetWindow(), packet ) );

			break;
		}
		default:
			break;
	}
}

void NetworkHandler::Tick() {
	if( !m_connected ) {
		return;
	}

	while( m_selector.wait( sf::microseconds( 1 ) ) ) {
		sf::Packet packet;
		m_socket.receive( packet );
		HandlePacket( packet );
	}
}

bool NetworkHandler::IsAuthenticated() {
	return ( m_connected && m_authenticated );
}

bool NetworkHandler::IsConnected() {
	return m_connected;
}
