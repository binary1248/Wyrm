#include "items/item.h"
#include "player.h"
#include "utility.h"
#include "inventory.h"

Inventory::Inventory() {
  isDirty = true;
}

Inventory::~Inventory() {
  for( std::list< std::pair<Item*, size_t> >::iterator i = items.begin(); i != items.end(); ) {
    if( i->first ) {
      delete i->first;
    }

    i = items.erase(i);
  }
}

void Inventory::SendUpdate(Player* p) {
  while( !send_buffer.empty() ) {
    p->SendPacket( *(send_buffer.front()) );
    delete send_buffer.front();
    send_buffer.pop_front();
  }

  ClearDirty();
}

void Inventory::AddItem(Item* item, size_t amount) {
  sf::Packet* packet = new sf::Packet();

  for( std::list< std::pair<Item*, size_t> >::iterator i = items.begin(); i != items.end(); i++ ) {
    if( item == i->first ) {
      i->second += amount;

      (*packet) << (sf::Uint16)SERVER_INVENTORY << (sf::Uint16)INVENTORY_CHANGE
                << sf::String(i->first->GetType()) << sf::String(i->first->GetName()) << i->second;
      send_buffer.push_back( packet );

      return;
    }
  }

  items.push_back( std::make_pair<Item*, size_t>(item, amount) );

  (*packet) << (sf::Uint16)SERVER_INVENTORY << (sf::Uint16)INVENTORY_ADD
            << sf::String(item->GetType()) << sf::String(item->GetName()) << amount;
  send_buffer.push_back( packet );
}

void Inventory::RemoveItem(Item* item, size_t amount) {
  sf::Packet* packet = new sf::Packet();

  for( std::list< std::pair<Item*, size_t> >::iterator i = items.begin(); i != items.end(); i++ ) {
    if( item == i->first ) {
      if( amount > i->second ) {
        LogConsole( "Tried to remove more items than in inventory." );
      } else {
        i->second -= amount;

        (*packet) << (sf::Uint16)SERVER_INVENTORY << (sf::Uint16)INVENTORY_CHANGE
                  << sf::String(i->first->GetType()) << sf::String(i->first->GetName()) << i->second;
        send_buffer.push_back( packet );
      }
      if( i->second == 0 ) {
        items.erase( i );
        (*packet) << (sf::Uint16)SERVER_INVENTORY << (sf::Uint16)INVENTORY_REMOVE
                  << sf::String(item->GetType()) << sf::String(item->GetName());
        send_buffer.push_back( packet );
      }
      break;
    }
  }
}
