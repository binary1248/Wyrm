#include <items/item.hpp>
#include <player.hpp>
#include <utility.hpp>
#include <inventory.hpp>

Inventory::Inventory() :
	m_dirty( true ) {
}

Inventory::~Inventory() {
}

void Inventory::ClearDirty() {
	m_dirty = false;
}

bool Inventory::IsDirty() const {
	return m_dirty;
}

void Inventory::SendUpdate( PlayerPtr player ) {
	if( !IsDirty() ) {
		return;
	}

	while( !m_send_buffer.empty() ) {
		player->SendPacket( m_send_buffer.front() );
		m_send_buffer.pop();
	}

	ClearDirty();
}

void Inventory::AddItem( ItemPtr item, size_t amount ) {
	PacketPtr packet = std::make_shared<sf::Packet>();

	std::list< std::pair<ItemPtr, size_t> >::iterator iter( m_items.begin() );
	std::list< std::pair<ItemPtr, size_t> >::iterator end( m_items.end() );

	for( ; iter != end; ++iter ) {
		if( item == iter->first ) {
			iter->second += amount;

			(*packet) << static_cast<sf::Uint16>( ServerToClient::SERVER_INVENTORY ) << static_cast<sf::Uint16>( ServetToClientInventory::INVENTORY_CHANGE )
			          << sf::String( iter->first->GetType() ) << sf::String( iter->first->GetName() )
			          << static_cast<sf::Uint32>( iter->second );
			m_send_buffer.push( packet );

			return;
		}
	}

	m_items.push_back( std::pair<ItemPtr, size_t>( item, amount ) );

	(*packet) << static_cast<sf::Uint16>( ServerToClient::SERVER_INVENTORY ) << static_cast<sf::Uint16>( ServetToClientInventory::INVENTORY_ADD )
	          << sf::String( item->GetType() ) << sf::String( item->GetName() )
	          << static_cast<sf::Uint32>( amount );
	m_send_buffer.push( packet );
}

void Inventory::RemoveItem( ItemPtr item, size_t amount ) {
	PacketPtr packet = std::make_shared<sf::Packet>();

	std::list< std::pair<ItemPtr, size_t> >::iterator iter( m_items.begin() );
	std::list< std::pair<ItemPtr, size_t> >::iterator end( m_items.end() );

	for( ; iter != end; ++iter ) {
		if( item == iter->first ) {
			if( amount > iter->second ) {
				LogConsole( "Tried to remove more items than in inventory." );
			} else {
				iter->second -= amount;

				(*packet) << static_cast<sf::Uint16>( ServerToClient::SERVER_INVENTORY ) << static_cast<sf::Uint16>( ServetToClientInventory::INVENTORY_CHANGE )
				          << sf::String( iter->first->GetType() ) << sf::String( iter->first->GetName() )
				          << static_cast<sf::Uint32>( iter->second );
				m_send_buffer.push( packet );
			}

			if( iter->second == 0 ) {
				m_items.erase( iter );
				(*packet) << static_cast<sf::Uint16>( ServerToClient::SERVER_INVENTORY ) << static_cast<sf::Uint16>( ServetToClientInventory::INVENTORY_REMOVE )
				          << sf::String( item->GetType() ) << sf::String( item->GetName() );
				m_send_buffer.push( packet );
			}

			break;
		}
	}
}
