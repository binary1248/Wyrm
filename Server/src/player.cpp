#include <iostream>

#include <boost/make_shared.hpp>

#include "game.h"
#include "utility.h"
#include "inventory.h"
#include "items/item.h"
#include "networkmanager.h"
#include "auth.h"
#include "player.h"

Player::Player( ObjectPtr agent ) :
	m_id( Game::GetGame()->GetPlayerManager()->NewID() ),
	m_agent(),
	m_delete_me( false ),
	m_half_open( true ),
	m_inventory( boost::make_shared<Inventory>() ) {

  LogConsole( "Player " + STRING_CAST( m_id ) + " created" );

  // Set agent
  SetAgent( agent );

  ItemPtr some_item = boost::make_shared<Item>( "Some Item", "Some Type" );
  ItemPtr another_item = boost::make_shared<Item>( "Another Item", "Another Type" );

  m_inventory->AddItem( some_item, 3 );
  m_inventory->AddItem( another_item, 1 );
  m_inventory->RemoveItem( some_item, 1 );
}

Player::~Player() {
	ObjectPtr agent( m_agent.lock() );
	if( agent ) {
		agent->Delete();
	}

  LogConsole( "Player " + STRING_CAST( m_id ) + " destroyed" );
}

void Player::Update() {
  m_inventory->SendUpdate( shared_from_this() );

  FlushBuffer();
  HandleSocketData();

  ObjectPtr agent( m_agent.lock() );
	if( !agent ) {
		LogConsole( "Player agent deleted, deleting player." );
		Delete();
		return;
	}
}

void Player::Send( PacketPtr packet ) {
  Game::GetGame()->GetNetworkManager()->SendData( shared_from_this(), packet );
}

void Player::FlushBuffer() {
  if( IsDeleted() || m_half_open ) {
    return;
  }

  while( !m_send_buffer.empty() ) {
    Send( m_send_buffer.front() );
    m_send_buffer.pop();
  }
}

void Player::SendPacket( PacketPtr packet, bool prio ) {
  if( IsDeleted() ) {
    return;
  }

  if( prio ) {
    Send( packet );
  } else {
    m_send_buffer.push( packet );
  }
}

void Player::ReceivePacket( PacketPtr packet ) {
  m_recv_buffer.push( packet );
}

void Player::HandleSocketData() {
  while( !m_recv_buffer.empty() ) {
		PacketPtr packet = m_recv_buffer.front();

		if( !m_half_open ) {
			HandlePacket( packet );
		} else {
			LogConsole( "Client " + STRING_CAST( m_id ) + " sent auth data" );

			Auth( packet );
		}

		m_recv_buffer.pop();
  }
}

void Player::HandlePacket( PacketPtr packet ) {
  if( packet->GetDataSize() < 4 ) {
    LogConsole("Player sent packet size < 4");
    return;
  }

  ObjectPtr agent( m_agent.lock() );
	if( !agent ) {
		LogConsole( "Player agent deleted, deleting player." );
		Delete();
		return;
	}

  sf::Uint16 type0;
  (*packet) >> type0;

  switch( type0 ) {
    case CLIENT_COMMAND:
      agent->HandlePacket( packet );
      break;
    default:
      LogConsole( "Player sent packet with type0=" + STRING_CAST( type0 ) );
      break;
  }
}

void Player::Auth( PacketPtr packet ) {
	PacketPtr reply = boost::make_shared<sf::Packet>();

  if( CheckAuth( packet ) ) {
    (*reply) << sf::String("Authentication successful");
    SendPacket( reply, true );
    LogConsole("Authentication successful");

    m_half_open = false;
  } else {
    (*reply) << sf::String("Authentication failed");
    SendPacket( reply, true );
    LogConsole( "Authentication failed: " + GetName() + " disconnected (auth failure).." );

    Delete();
  }
}

void Player::Delete() {
	m_delete_me = true;
}

bool Player::IsDeleted() const {
	return m_delete_me;
}

sf::Uint16 Player::GetId() const {
	return m_id;
}

ObjectWeakPtr Player::GetAgent() const {
	return m_agent;
}

void Player::SetAgent( ObjectPtr agent ) {
	if( !agent ) {
		LogConsole( "Tried to set non-existing agent, deleting player." );
		Delete();
		return;
	}

  m_agent = ObjectWeakPtr( agent );
  agent->SetName( GetName() );
  PacketPtr packet = boost::make_shared<sf::Packet>();
  (*packet) << static_cast<sf::Uint16>( SERVER_SET_ID ) << agent->GetId();
  SendPacket( packet );
}

const sf::String& Player::GetName() const {
	return m_name;
}

void Player::SetName( std::string name ) {
	m_name = name;
}

InventoryWeakPtr Player::GetInventory() const {
	return InventoryWeakPtr( m_inventory );
}
