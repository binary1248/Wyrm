#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "objectmanager.h"
#include "playermanager.h"
#include "networkmanager.h"

class Game {
  public:
    Game();
    ~Game();
    int Run();

  private:
    bool running;

    PlayerManager* playermanager;
    ObjectManager* objectmanager;
    NetworkManager* networkmanager;
};

#endif // GAME_H_INCLUDED
