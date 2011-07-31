#include "items/item.h"
#include "player.h"
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

  ClearDirty();
}
