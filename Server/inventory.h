#ifndef INVENTORY_H_INCLUDED
#define INVENTORY_H_INCLUDED

#include <utility>
#include <list>
#include "networkmanager.h"

class Item;
class Player;

class Inventory {
  public:
    Inventory();
    ~Inventory();

    inline void ClearDirty() { isDirty = false; }
    inline bool IsDirty() { return isDirty; }

    void SendUpdate(Player* p);

    void AddItem(Item* item, size_t amount);
    void RemoveItem(Item* item, size_t amount);
  private:
    bool isDirty;

    std::list< std::pair<Item*, size_t> > items;

    std::list<sf::Packet*> send_buffer;
};

enum packet_server_inventory {
  INVENTORY_ADD = 0,
  INVENTORY_REMOVE,
  INVENTORY_CHANGE
};

#endif // INVENTORY_H_INCLUDED
