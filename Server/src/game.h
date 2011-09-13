#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <boost/shared_ptr.hpp>

#include "objectmanager.h"
#include "playermanager.h"
#include "networkmanager.h"

class Game {
  public:
    ~Game();

    static boost::shared_ptr<Game> GetGame();

    int Run();
    void Stop();

    PlayerManagerPtr GetPlayerManager() const;
    ObjectManagerPtr GetObjectManager() const;
    NetworkManagerPtr GetNetworkManager() const;

  private:
    Game();

    void Tick(float time);

    static boost::shared_ptr<Game> instance;

    bool m_running;

    PlayerManagerPtr m_playermanager;
    ObjectManagerPtr m_objectmanager;
    NetworkManagerPtr m_networkmanager;
};

typedef boost::shared_ptr<Game> GamePtr;

#endif // GAME_H_INCLUDED
