#ifndef INVENTORY_H_INCLUDED
#define INVENTORY_H_INCLUDED

#include <utility>
#include <list>
#include <SFML/Network.hpp>

class Item;

class Inventory {
  public:
    Inventory();
    ~Inventory();

    void HandlePacket( sf::Packet& p );

    void AddItem(Item* item, size_t amount);
    void RemoveItem(Item* item, size_t amount);
  private:
    std::list< std::pair<Item*, size_t> > items;
};

enum packet_server_inventory {
  INVENTORY_ADD = 0,
  INVENTORY_REMOVE,
  INVENTORY_CHANGE
};

#endif // INVENTORY_H_INCLUDED
