#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include <objectmanager.hpp>
#include <resourcemanager.hpp>
#include <gui.hpp>
#include <network.hpp>
#include <player.hpp>
#include <system.hpp>

class Game {
  public:
    ~Game();

    static const std::shared_ptr<Game>& GetGame();

    void Run();
    void Stop();

    const PlayerPtr& GetPlayer() const;
    const PlayerPtr& CreatePlayer(sf::Uint16 id_, std::string name_);

    const GUIPtr& GetGUI() const;
    const NetworkHandlerPtr& GetNetworkHandler() const;
    const ObjectManagerPtr& GetObjectManager() const;
    const ResourceManagerPtr& GetResourceManager() const;
    const SystemPtr& GetSystem() const;
    void SetSystem( const SystemPtr& system );

    void Resize( float width, float height );

    const sf::Vector2f GetDefaultResolution() const;
    const sf::Vector2f GetCurrentResolution() const;
    const sf::View& GetCurrentView() const;
    const std::shared_ptr<sf::RenderWindow>& GetWindow() const;

  private:
    Game();
    void Tick( float time );

    bool m_running;

    static std::shared_ptr<Game> m_instance;

    std::shared_ptr<sf::RenderWindow> m_window;
    NetworkHandlerPtr m_networkhandler;
    ObjectManagerPtr m_objectmanager;
    ResourceManagerPtr m_resourcemanager;
    SystemPtr m_system;
    GUIPtr m_gui;

    PlayerPtr m_player;
};

typedef std::shared_ptr<Game> GamePtr;

#endif // GAME_HPP_INCLUDED
