#ifndef INVENTORY_HPP_INCLUDED
#define INVENTORY_HPP_INCLUDED

#include <utility>
#include <list>
#include <queue>
#include <memory>

#include <networkmanager.hpp>

class Player;
class Item;

typedef std::shared_ptr<Player> PlayerPtr;
typedef std::shared_ptr<Item> ItemPtr;

class Inventory {
  public:
    Inventory();
    ~Inventory();

    void SendUpdate( PlayerPtr player );

    void AddItem( ItemPtr item, size_t amount );
    void RemoveItem( ItemPtr item, size_t amount );

  private:
		void ClearDirty();
    bool IsDirty() const;

    bool m_dirty;

    std::list< std::pair<ItemPtr, size_t> > m_items;

    std::queue<PacketPtr> m_send_buffer;
};

typedef std::shared_ptr<Inventory> InventoryPtr;
typedef std::weak_ptr<Inventory> InventoryWeakPtr;

enum packet_server_inventory {
  INVENTORY_ADD = 0,
  INVENTORY_REMOVE,
  INVENTORY_CHANGE
};

#endif // INVENTORY_HPP_INCLUDED
