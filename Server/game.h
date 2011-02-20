#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <SFML/System.hpp>

#include "objectmanager.h"
#include "playermanager.h"
#include "networkmanager.h"

#define TIME Game::GetGame()->GetTime()

class Game {
  public:
    static Game* GetGame();
    ~Game();
    int Run();

    void Tick(float time);

    inline PlayerManager* GetPlayerManager() { return playermanager; }
    inline ObjectManager* GetObjectManager() { return objectmanager; }
    inline NetworkManager* GetNetworkManager() { return networkmanager; }

    std::string GetTime();

  private:
    Game();

    static Game* instance;

    bool running;

    PlayerManager* playermanager;
    ObjectManager* objectmanager;
    NetworkManager* networkmanager;

    sf::Clock clock;
};

#endif // GAME_H_INCLUDED
