#ifndef PLAYERMANAGER_H_INCLUDED
#define PLAYERMANAGER_H_INCLUDED

#include <list>

#include <boost/shared_ptr.hpp>

#include <SFML/Network.hpp>

#include "player.h"

class Game;
class Object;

class PlayerManager {
  public:
    PlayerManager();
    ~PlayerManager();

    void Tick(float time);

    PlayerPtr CreatePlayer();

    sf::Uint16 NewID();

  private:
    std::list<PlayerPtr> m_players;
    sf::Uint16 m_lastId;
};

typedef boost::shared_ptr<PlayerManager> PlayerManagerPtr;

#endif // PLAYERMANAGER_H_INCLUDED
