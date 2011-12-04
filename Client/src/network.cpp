#include <SFML/Network.hpp>

#include <config.hpp>
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

int NetworkHandler::Connect( sf::String username, sf::String password ) {
  if ( m_socket.Connect( "localhost", 1337 ) != sf::Socket::Done ) {
    // Error...
    std::cout << "Can't connect to server...\n";
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
    std::cout << string.ToAnsiString() << version_major << " " << version_minor << "\n";
    if( !(string.ToAnsiString().compare( "Wyrm protocol version " )) &&
         ( version_major == PROTOCOL_VER_MAJOR ) &&
         ( version_minor == PROTOCOL_VER_MINOR ) ) {
      sf::Packet response;
      response << username << password;
      m_socket.Send( response );
    } else {
      std::cout << "Protocol mismatch\n";
      Disconnect();
      return -1;
    }
  }

  {
    sf::Packet packet;
    if( m_socket.Receive(packet) != sf::TcpSocket::Done ) {
      std::cout << "Error while receiving.\n";
      Disconnect();
      return -1;
    }
    sf::String s;
    packet >> s;
    if( !s.ToAnsiString().compare("Authentication successful") ) {
      std::cout << "Authentication successful\n";
      m_authenticated = true;
    } else {
      std::cout << "Authentication failed: " << s.ToAnsiString() << "\n";
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
    case SERVER_OBJECT: {
      Game::GetGame()->GetObjectManager()->DispatchPacket( packet );
      break;
    }
    case SERVER_SET_ID: {
      sf::Uint16 id;
      packet >> id;
      PlayerPtr player = Game::GetGame()->GetPlayer();
      if( !player ) {
        player = Game::GetGame()->CreatePlayer( 0, "" );
      }
      player->SetShip( id );
      break;
    }
    case SERVER_INVENTORY: {
      PlayerPtr player = Game::GetGame()->GetPlayer();
      if( player ) {
        player->GetInventory()->HandlePacket( packet );
      }
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
