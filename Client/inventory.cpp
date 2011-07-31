#include "items/item.h"
#include "inventory.h"

Inventory::Inventory() {

}

Inventory::~Inventory() {
  for( std::list< std::pair<Item*, size_t> >::iterator i = items.begin(); i != items.end(); ) {
    if( i->first ) {
      delete i->first;
    }

    i = items.erase(i);
  }
}

void Inventory::HandlePacket( sf::Packet& p ) {

}

void Inventory::AddItem(Item* item, size_t amount) {
  for( std::list< std::pair<Item*, size_t> >::iterator i = items.begin(); i != items.end(); ) {
    if( item == i->first ) {
      i->second+=amount;
      delete item;
      return;
    }
  }

  items.push_back( std::make_pair<Item*, size_t>(item, amount) );
}

void Inventory::RemoveItem(Item* item, size_t amount) {

}
