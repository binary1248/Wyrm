#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include <memory>

#include <objectmanager.hpp>
#include <playermanager.hpp>
#include <networkmanager.hpp>

class Game {
  public:
    ~Game();

    static std::shared_ptr<Game> GetGame();

    int Run();
    void Stop();

    const PlayerManagerPtr& GetPlayerManager() const;
    const ObjectManagerPtr& GetObjectManager() const;
    const NetworkManagerPtr& GetNetworkManager() const;

  private:
    Game();

    void Tick( float time );

    static std::shared_ptr<Game> m_instance;

    ObjectManagerPtr m_objectmanager;
    PlayerManagerPtr m_playermanager;
    NetworkManagerPtr m_networkmanager;

    bool m_running;
};

typedef std::shared_ptr<Game> GamePtr;

#endif // GAME_HPP_INCLUDED
