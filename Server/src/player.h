#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <queue>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <SFML/Network.hpp>

#include "inventory.h"
#include "objects/object.h"

class Player : public boost::enable_shared_from_this<Player> {
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
    ObjectWeakPtr GetAgent() const;
    void SetAgent( ObjectPtr agent );

    const sf::String& GetName() const;
    void SetName( std::string name );

    InventoryWeakPtr GetInventory() const;
  private:
    void Send( PacketPtr p);
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

    std::queue<PacketPtr> m_send_buffer;
    std::queue<PacketPtr> m_recv_buffer;

    // Character vars
    ObjectWeakPtr m_agent;
    InventoryPtr m_inventory;
};

typedef boost::shared_ptr<Player> PlayerPtr;
typedef boost::weak_ptr<Player> PlayerWeakPtr;

enum packet_client_command {
  COMMAND_CONTROL = 0
};

#endif // PLAYER_H_INCLUDED
