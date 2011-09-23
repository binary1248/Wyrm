#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <boost/scoped_ptr.hpp>

#include "objectmanager.h"
#include "resourcemanager.h"
#include "gui.h"
#include "network.h"
#include "player.h"
#include "backdrop.h"

class Game {
  public:
    ~Game();

    static boost::shared_ptr<Game> GetGame();

    void Run();
    void Stop();

    GUIPtr GetGUI();
    PlayerPtr GetPlayer();
    PlayerPtr CreatePlayer(sf::Uint16 id_, std::string name_);
    NetworkHandlerPtr GetNetworkHandler();
    ObjectManagerPtr GetObjectManager();
    ResourceManagerPtr GetResourceManager();
    BackdropPtr GetBackdrop();
    void Resize( float width, float height );
    sf::Vector2f GetDefaultResolution();
    sf::Vector2f GetCurrentResolution();
  private:
    Game();
    void Tick(float t);
    bool bRunning;

    static boost::shared_ptr<Game> instance;

    boost::shared_ptr<sf::RenderWindow> App;
    NetworkHandlerPtr networkhandler;
    ObjectManagerPtr objectmanager;
    ResourceManagerPtr resourcemanager;
    BackdropPtr backdrop;
    GUIPtr gui;

    PlayerPtr player;
};

typedef boost::shared_ptr<Game> GamePtr;

#endif // GAME_H_INCLUDED
