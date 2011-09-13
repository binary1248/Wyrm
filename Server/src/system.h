#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include <list>
#include <utility>

#include <boost/shared_ptr.hpp>

#include "objects/object.h"
#include "player.h"

enum object_state{
  CLEAN = 0,
  DIRTY_PARTIAL = 1,
  DIRTY_FULL = 2,
  DELETED = 3,
};

class System {
  public:
    System( sf::Uint16 id, std::string name );
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

    std::list<PlayerWeakPtr> m_players;
    std::list<ObjectWeakPtr> m_objects;
};

typedef boost::shared_ptr<System> SystemPtr;

#endif // SYSTEM_H_INCLUDED
