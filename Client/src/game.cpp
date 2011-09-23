#include <sstream>
#include <GL/gl.h>
#include <GL/glu.h>

#include <boost/make_shared.hpp>

#include <SFML/Window.hpp>

#include "game.h"
#include "backdrop.h"
#include "events.h"

GamePtr Game::instance;

#define DEFAULT_WIDTH 1024
#define DEFAULT_HEIGHT 768

Game::Game() {
  // Create the main rendering window
  App = boost::make_shared<sf::RenderWindow>( sf::VideoMode(DEFAULT_WIDTH, DEFAULT_HEIGHT, 32),
																						  "WYRM",
																						  sf::Style::Titlebar | sf::Style::Close,
																						  sf::ContextSettings(24,    // Depth buffer
																																	8,    // Stencil buffer
																																	8) ); // AA level

	App->EnableVerticalSync( true );

  srand( time(NULL) );

  LoadKeymap();
  App->EnableKeyRepeat(true);

  gui = boost::make_shared<GUI>( App );

  resourcemanager = boost::make_shared<ResourceManager>();
  objectmanager = boost::make_shared<ObjectManager>();
  networkhandler = boost::make_shared<NetworkHandler>();

  bRunning = true;
}

Game::~Game() {
}

GamePtr Game::GetGame() {
  if( !instance ) {
    instance.reset( new Game() );
  }

  return instance;
}

void Game::Run() {
  static unsigned int counter = 0;
  static sf::Clock fps_timer;
  while ( bRunning && App->IsOpened() ) {
    float ElapsedTime = (float)(App->GetFrameTime()) / 1000.0f;
    Tick(ElapsedTime);

    counter++;

    if( fps_timer.GetElapsedTime() > 1000 ) {
      fps_timer.Reset();
      std::stringstream ss;
      ss << "WYRM - " << counter << " FPS";
      counter = 0;
      App->SetTitle( ss.str() );
    }

    App->SetFramerateLimit(200);
  }
}

void Game::Stop() {
  bRunning = false;
}

GUIPtr Game::GetGUI() {
  return gui;
}

PlayerPtr Game::GetPlayer() {
  return player;
}

PlayerPtr Game::CreatePlayer( sf::Uint16 id, std::string name ) {
  if( !player ) {
    player = boost::make_shared<Player>( id , name );
  }

  return player;
}

NetworkHandlerPtr Game::GetNetworkHandler() {
  return networkhandler;
}

ObjectManagerPtr Game::GetObjectManager() {
  return objectmanager;
}

ResourceManagerPtr Game::GetResourceManager() {
  return resourcemanager;
}

BackdropPtr Game::GetBackdrop() {
  return backdrop;
}

void Game::Resize( float width, float height ) {
  sf::View view( sf::FloatRect(0, 0, width, height) );
  App->SetView( view );
}

sf::Vector2f Game::GetDefaultResolution() {
  return sf::Vector2f( DEFAULT_WIDTH, DEFAULT_HEIGHT );
}

sf::Vector2f Game::GetCurrentResolution() {
  sf::View view = App->GetView();
  return view.GetSize();
}

void Game::Tick(float t) {

  if( HandleEvents(*App) ) {
    bRunning = false;
  }

  networkhandler->Tick();

  // Clear the screen (fill it with black color)
  App->Clear(sf::Color(0, 0, 0));

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if( networkhandler->IsAuthenticated() ) {
    if( !backdrop ) {
      backdrop = boost::make_shared<Backdrop>( App );
    }

    objectmanager->Tick(t);
    if( player ) {
      player->Tick(t);
    }

    backdrop->Draw(*App);

    objectmanager->DrawAll(*App);
  }

  gui->Draw(*App);

  // Display window contents on screen
  App->Display();
}
