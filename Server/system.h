#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include <vector>
#include <utility>

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
    System(sf::Uint16 id, std::string name);
    ~System();

    void AddPlayer(Player* p);
    void AddObject(Object* o);

    void Tick(float time);

    inline sf::Uint16 GetId() { return id; }
    inline std::string GetName() { return name; }
  private:
    void RemoveDeletedPlayers();
    void RemoveObject(Object* o);

    sf::Uint16 id;
    std::string name;

    std::vector<Player*> players;
    std::vector< std::pair<Object*,char> > objects;
};

#endif // SYSTEM_H_INCLUDED
