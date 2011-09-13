#ifndef INVENTORY_H_INCLUDED
#define INVENTORY_H_INCLUDED

#include <utility>
#include <list>
#include <queue>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "networkmanager.h"

class Player;
class Item;

typedef boost::shared_ptr<Player> PlayerPtr;
typedef boost::shared_ptr<Item> ItemPtr;

class Inventory {
  public:
    Inventory();
    ~Inventory();

    void SendUpdate( PlayerPtr player );

    void AddItem( ItemPtr item, size_t amount );
    void RemoveItem( ItemPtr item, size_t amount );

  private:
		void ClearDirty();
    bool IsDirty();

    bool m_isDirty;

    std::list< std::pair<ItemPtr, size_t> > m_items;

    std::queue<PacketPtr> m_send_buffer;
};

typedef boost::shared_ptr<Inventory> InventoryPtr;
typedef boost::weak_ptr<Inventory> InventoryWeakPtr;

enum packet_server_inventory {
  INVENTORY_ADD = 0,
  INVENTORY_REMOVE,
  INVENTORY_CHANGE
};

#endif // INVENTORY_H_INCLUDED
