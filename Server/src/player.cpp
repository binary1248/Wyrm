#include <config.hpp>
#include <game.hpp>
#include <utility.hpp>
#include <inventory.hpp>
#include <items/item.hpp>
#include <networkmanager.hpp>
#include <auth.hpp>
#include <player.hpp>

Player::Player( ObjectPtr agent ) :
	m_id( Game::GetGame()->GetPlayerManager()->NewID() ),
	m_delete_me( false ),
	m_half_open( true ),
	m_agent(),
	m_inventory( std::make_shared<Inventory>() ) {

	LogConsole( "Player " + string_cast( m_id ) + " created" );

	// Set agent
	SetAgent( agent );

	ItemPtr some_item = std::make_shared<Item>( "Some Item", "Some Type" );
	ItemPtr another_item = std::make_shared<Item>( "Another Item", "Another Type" );

	m_inventory->AddItem( some_item, 3 );
	m_inventory->AddItem( another_item, 1 );
	m_inventory->RemoveItem( some_item, 1 );
}

Player::~Player() {
	ObjectPtr agent( m_agent.lock() );
	if( agent ) {
		agent->Delete();
	}

	LogConsole( "Player " + string_cast( m_id ) + " destroyed" );
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
		m_send_buffer.pop_front();
	}
}

void Player::SendPacket( PacketPtr packet, bool prio ) {
	if( IsDeleted() ) {
		return;
	}

	if( !prio ) {
		m_send_buffer.push_back( packet );
	} else {
		Send( packet );
	}
}

void Player::ReceivePacket( PacketPtr packet ) {
	m_recv_buffer.push_back( packet );
}

void Player::HandleSocketData() {
	while( !m_recv_buffer.empty() ) {
		PacketPtr packet = m_recv_buffer.front();

		if( !m_half_open ) {
			HandlePacket( packet );
		} else {
			LogConsole( "Client " + string_cast( m_id ) + " sent auth data" );

			Auth( packet );
		}

		m_recv_buffer.pop_front();
	}
}

void Player::HandlePacket( PacketPtr packet ) {
	if( packet->getDataSize() < 4 ) {
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
		case ClientToServer::CLIENT_COMMAND:
			agent->HandlePacket( packet );
			break;
		default:
			LogConsole( "Player sent packet with type0=" + string_cast( type0 ) );
			break;
	}
}

void Player::Auth( PacketPtr packet ) {
	PacketPtr reply = std::make_shared<sf::Packet>();

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

const ObjectWeakPtr& Player::GetAgent() const {
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
	PacketPtr packet = std::make_shared<sf::Packet>();
	(*packet) << static_cast<sf::Uint16>( ServerToClient::SERVER_SET_ID ) << agent->GetId();
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

void Player::LoadResource( sf::Uint32 id ) {
	if( !id || IsResourceLoaded( id ) ) {
		return;
	}

	PacketPtr packet( std::make_shared<sf::Packet>() );

	(*packet) << static_cast<sf::Uint16>( ServerToClient::SERVER_RESOURCE );

	Game::GetGame()->GetResourceManager()->FillResourcePacket( id, packet );

	SendPacket( packet );

	m_loaded_resources.insert( id );
}

bool Player::IsResourceLoaded( sf::Uint32 id ) const {
	if( m_loaded_resources.find( id ) == m_loaded_resources.end() ) {
		return false;
	}

	return true;
}
