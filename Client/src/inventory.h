#ifndef INVENTORY_H_INCLUDED
#define INVENTORY_H_INCLUDED

#include <utility>
#include <list>

#include <boost/shared_ptr.hpp>

#include <SFML/Network.hpp>

class Item;

typedef boost::shared_ptr<Item> ItemPtr;

class Inventory {
  public:
    Inventory();
    ~Inventory();

    void HandlePacket( sf::Packet& p );

    void AddItem(ItemPtr item, size_t amount);
    void RemoveItem(ItemPtr item, size_t amount);
  private:
    std::list< std::pair<ItemPtr, size_t> > items;
};

typedef boost::shared_ptr<Inventory> InventoryPtr;

enum packet_server_inventory {
  INVENTORY_ADD = 0,
  INVENTORY_REMOVE,
  INVENTORY_CHANGE
};

#endif // INVENTORY_H_INCLUDED
