#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "objectmanager.h"
#include "gui.h"
#include "network.h"
#include "player.h"
#include "backdrop.h"

class Game {
  public:
    Game();
    ~Game();

    static Game* GetGame();

    void Run();
    void Stop();

    GUI* GetGUI();
    Player* GetPlayer();
    Player* CreatePlayer(sf::Uint16 id_, std::string name_);
    NetworkHandler* GetNetworkHandler();
    ObjectManager* GetObjectManager();
    Backdrop* GetBackdrop();
  private:
    void Tick(float t);
    bool bRunning;

    static Game* instance;

    sf::RenderWindow* App;
    NetworkHandler* networkhandler;
    ObjectManager* objectmanager;
    Backdrop* backdrop;
    GUI* gui;

    Player* player;
};

#endif // GAME_H_INCLUDED
