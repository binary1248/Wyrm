#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include <vector>
#include <memory>
#include <set>

#include <SFML/Network.hpp>

#include <inventory.hpp>
#include <objects/object.hpp>

class Player : public std::enable_shared_from_this<Player> {
	public:
		Player( ObjectPtr agent );
		~Player();

		// Network Handlers
		void Update();
		void SendPacket( PacketPtr packet, bool prio = false );
		void ReceivePacket( PacketPtr packet );

		void Delete();
		bool IsDeleted() const;

		// ID Handlers
		sf::Uint16 GetId() const;
		const ObjectWeakPtr& GetAgent() const;
		void SetAgent( ObjectPtr agent );

		const sf::String& GetName() const;
		void SetName( std::string name );

		InventoryWeakPtr GetInventory() const;

		void LoadResource( sf::Uint32 id );
		bool IsResourceLoaded( sf::Uint32 id ) const;
	private:
		void Send( PacketPtr p );
		void HandleSocketData();
		void FlushBuffer();
		void HandlePacket( PacketPtr packet );

		void Auth( PacketPtr packet );

		// ID vars
		sf::Uint16 m_id;
		sf::String m_name;

		// Network vars
		SocketWeakPtr m_connection;

		bool m_delete_me;
		bool m_half_open;

		std::deque<PacketPtr> m_send_buffer;
		std::deque<PacketPtr> m_recv_buffer;

		// Character vars
		ObjectWeakPtr m_agent;
		InventoryPtr m_inventory;

		// Resource control vars
		std::set<sf::Uint32> m_loaded_resources;
};

typedef std::shared_ptr<Player> PlayerPtr;
typedef std::weak_ptr<Player> PlayerWeakPtr;

enum ClientToServerCommand {
	COMMAND_CONTROL = 0
};

#endif // PLAYER_HPP_INCLUDED
