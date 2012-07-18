#ifndef INVENTORY_HPP_INCLUDED
#define INVENTORY_HPP_INCLUDED

#include <utility>
#include <list>
#include <memory>

#include <SFML/Network.hpp>

class Item;

typedef std::shared_ptr<Item> ItemPtr;

class Inventory {
	public:
		Inventory();
		~Inventory();

		void HandlePacket( sf::Packet& packet );

		void AddItem( ItemPtr item, size_t amount );
		void RemoveItem( ItemPtr item, size_t amount );

	private:
		std::list< std::pair<ItemPtr, size_t> > m_items;
};

typedef std::shared_ptr<Inventory> InventoryPtr;

enum ServerToClientInventory {
	INVENTORY_ADD = 0,
	INVENTORY_REMOVE,
	INVENTORY_CHANGE
};

#endif // INVENTORY_HPP_INCLUDED
