#include <boost/make_shared.hpp>

#include "items/item.h"
#include "player.h"
#include "utility.h"
#include "inventory.h"

Inventory::Inventory() :
	m_isDirty( true ) {
}

Inventory::~Inventory() {
}

void Inventory::ClearDirty() {
	m_isDirty = false;
}

bool Inventory::IsDirty() {
	return m_isDirty;
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
  PacketPtr packet = boost::make_shared<sf::Packet>();

  for( std::list< std::pair<ItemPtr, size_t> >::iterator i = m_items.begin(); i != m_items.end(); i++ ) {
    if( item == i->first ) {
      i->second += amount;

      (*packet) << static_cast<sf::Uint16>( SERVER_INVENTORY ) << static_cast<sf::Uint16>( INVENTORY_CHANGE )
                << sf::String( i->first->GetType() ) << sf::String( i->first->GetName() ); static_cast<sf::Uint32>(i->second);
      m_send_buffer.push( packet );

      return;
    }
  }

  m_items.push_back( std::make_pair<ItemPtr, size_t>( item, amount ) );

  (*packet) << static_cast<sf::Uint16>( SERVER_INVENTORY ) << static_cast<sf::Uint16>( INVENTORY_ADD )
            << sf::String( item->GetType() ) << sf::String( item->GetName() ) << static_cast<sf::Uint32>( amount );
  m_send_buffer.push( packet );
}

void Inventory::RemoveItem( ItemPtr item, size_t amount ) {
  PacketPtr packet = boost::make_shared<sf::Packet>();

  for( std::list< std::pair<ItemPtr, size_t> >::iterator i = m_items.begin(); i != m_items.end(); i++ ) {
    if( item == i->first ) {
      if( amount > i->second ) {
        LogConsole( "Tried to remove more items than in inventory." );
      } else {
        i->second -= amount;

        (*packet) << static_cast<sf::Uint16>( SERVER_INVENTORY ) << static_cast<sf::Uint16>( INVENTORY_CHANGE )
                  << sf::String( i->first->GetType() ) << sf::String( i->first->GetName() ) << static_cast<sf::Uint32>( i->second );
        m_send_buffer.push( packet );
      }
      if( i->second == 0 ) {
        m_items.erase( i );
        (*packet) << static_cast<sf::Uint16>( SERVER_INVENTORY ) << static_cast<sf::Uint16>( INVENTORY_REMOVE )
                  << sf::String( item->GetType() ) << sf::String( item->GetName() );
        m_send_buffer.push( packet );
      }
      break;
    }
  }
}
