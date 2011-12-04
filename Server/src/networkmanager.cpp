#include <SFML/Network.hpp>

#include <config.hpp>
#include <game.hpp>
#include <player.hpp>
#include <utility.hpp>
#include <networkmanager.hpp>

std::string NetworkManager::ErrCode( sf::Socket::Status status ) {
  switch( status ) {
    case sf::TcpSocket::Disconnected:
      return "Disconnected";
    case sf::TcpSocket::Done:
      return "Done";
    case sf::TcpSocket::Error:
      return "Error";
    case sf::TcpSocket::NotReady:
      return "Not Ready";
    default:
      return "Undefined";
  }
}

NetworkManager::NetworkManager() :
	m_listening( false ) {
  m_selector.Clear();

  if( m_sock_listener.Listen( 1337 ) != sf::Socket::Done ) {
    LogConsole( "Failed to listen on port 1337" );
  } else {
		m_selector.Add( m_sock_listener );
		m_listening = true;
  }
}

NetworkManager::~NetworkManager() {
}

void NetworkManager::Tick( float /*time*/ ) {
  if( m_selector.Wait( 1 ) ) {
    if( m_selector.IsReady( m_sock_listener ) ) {
      AcceptSocket();
    }

    for( PlayerMap::iterator iter = m_players.begin(); iter != m_players.end(); ) {
    	PlayerPtr shared_player( ( iter->first ).lock() );
			SocketPtr shared_socket( iter->second );

    	if( shared_player && shared_socket ) {
				if( m_selector.IsReady( *shared_socket ) ) {
					ReceiveData( shared_player, shared_socket );
				}
    	} else {
    		m_selector.Remove( *shared_socket );
				shared_socket->Disconnect();

				m_players.erase( iter++ );
				continue;
    	}

    	++iter;
    }
  }
}

void NetworkManager::ReceiveData( PlayerPtr player, SocketPtr socket ) {
  PacketPtr packet = std::make_shared<sf::Packet>();
  sf::Socket::Status status = socket->Receive( *packet );

  switch( status ) {
    case sf::Socket::Disconnected:
      LogConsole( "Client " + string_cast( player->GetId() ) + " connection reset" );
      player->Delete();
      break;
    case sf::Socket::Done:
      player->ReceivePacket( packet );
      break;
    default:
      LogConsole( "Player socket status: " + ErrCode( status ) + "..." );
      break;
  }
}

void NetworkManager::SendData( PlayerPtr player, PacketPtr packet ) {
  SocketPtr socket = m_players[ PlayerWeakPtr( player ) ];
  sf::Socket::Status status = socket->Send( *packet );

  if( status != sf::TcpSocket::Done ) {
    LogConsole( "Failed sending data to client " + string_cast( player->GetId() ) + ": " + ErrCode( status ) );
  }
}

bool NetworkManager::IsListening() const {
  return m_listening;
}

void NetworkManager::AddPlayer( PlayerPtr player, SocketPtr socket ) {
	PlayerWeakPtr weak_player( player );

  if( m_players.find( weak_player ) != m_players.end() ) {
    LogConsole( "NetworkManager tried to insert existing player" );
    return;
  }

  m_players.insert( std::pair<PlayerWeakPtr, SocketPtr>( weak_player, socket ) );
  m_selector.Add( *socket );

  LogConsole( player->GetName() + " (" + socket->GetRemoteAddress().ToString() + ") connected.." );
}

void NetworkManager::AcceptSocket() {
  SocketPtr client = std::make_shared<sf::TcpSocket>();

  if ( m_sock_listener.Accept( *client ) != sf::Socket::Done ) {
    LogConsole( "NetworkManager Accept error..." );
  } else {
    // Send protocol info
    sf::Packet packet;
    packet << sf::String( "Wyrm protocol version " ) << static_cast<float>( PROTOCOL_VER_MAJOR ) << static_cast<float>( PROTOCOL_VER_MINOR );
    sf::Socket::Status status = client->Send( packet );

    if( status != sf::TcpSocket::Done ) {
      LogConsole( "Failed sending protocol version: " + ErrCode( status ) );
    } else {
			PlayerPtr player = Game::GetGame()->GetPlayerManager()->CreatePlayer();

			if( player ) {
				AddPlayer( player, client );
			} else {
				LogConsole( "Couldn't create new player, disconnecting." );
				client->Disconnect();
			}
    }
  }
}
