#ifndef SYSTEM_HPP_INCLUDED
#define SYSTEM_HPP_INCLUDED

#include <list>
#include <utility>
#include <memory>

#include <objects/object.hpp>
#include <player.hpp>

enum ObjectState {
  CLEAN = 0,
  DIRTY_PARTIAL = 1,
  DIRTY_FULL = 2,
  DELETED = 3
};

class System {
  public:
    System( sf::Uint16 id, std::string name, sf::Uint32 background_resource_id );
    ~System();

    void AddPlayer( PlayerPtr player );
    void AddObject( ObjectPtr object );

    void Tick( float time );

    sf::Uint16 GetId() const;
    std::string GetName() const;

  private:
    void RemoveDeletedPlayers();

    sf::Uint16 m_id;
    std::string m_name;

    sf::Uint32 m_background_resource_id;

    std::list<PlayerWeakPtr> m_players;
    std::list<ObjectWeakPtr> m_objects;
};

typedef std::shared_ptr<System> SystemPtr;

#endif // SYSTEM_HPP_INCLUDED
