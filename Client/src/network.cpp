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
  if ( m_socket.Connect( address.ToAnsiString(), 1337 ) != sf::Socket::Done ) {
    // Error...
    LogConsole( "Can't connect to server at " + address.ToAnsiString() + "." );
    return -1;
  }

  m_connected = true;

  m_selector.Add( m_socket );

  {
    sf::Packet packet;
    m_socket.Receive( packet );
    sf::String string;
    float version_major;
    float version_minor;
    packet >> string >> version_major >> version_minor;
    LogConsole( string.ToAnsiString() + string_cast( version_major ) + " " + string_cast( version_minor ) );
    if( !(string.ToAnsiString().compare( "Wyrm protocol version " )) &&
         ( version_major == PROTOCOL_VER_MAJOR ) &&
         ( version_minor == PROTOCOL_VER_MINOR ) ) {
      sf::Packet response;
      response << username << password;
      m_socket.Send( response );
    } else {
      LogConsole( "Protocol mismatch" );
      Disconnect();
      return -1;
    }
  }

  {
    sf::Packet packet;
    if( m_socket.Receive(packet) != sf::TcpSocket::Done ) {
      LogConsole( "Error while receiving." );
      Disconnect();
      return -1;
    }
    sf::String s;
    packet >> s;
    if( !s.ToAnsiString().compare("Authentication successful") ) {
      LogConsole( "Authentication successful" );
      m_authenticated = true;
    } else {
      LogConsole( "Authentication failed: " + s.ToAnsiString() );
      Disconnect();
      return -1;
    }
  }

  return 0;
}

void NetworkHandler::Disconnect() {
  m_selector.Remove( m_socket );
  m_socket.Disconnect();
}

void NetworkHandler::Send( sf::Packet packet ) {
  m_socket.Send( packet );
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

  while( m_selector.Wait( 1 ) ) {
    sf::Packet packet;
    m_socket.Receive( packet );
    HandlePacket( packet );
  }
}

bool NetworkHandler::IsAuthenticated() {
  return ( m_connected && m_authenticated );
}

bool NetworkHandler::IsConnected() {
  return m_connected;
}
