#include "game.h"
#include "backdrop.h"
#include "events.h"

Game* Game::instance;

Game::Game() {
  // Create the main rendering window
  App = new sf::RenderWindow(sf::VideoMode(1024, 768, 32),
                             "WYRM",
                             sf::Style::Titlebar | sf::Style::Close,
                             sf::ContextSettings(24,    // Depth buffer
                                                  8,    // Stencil buffer
                                                  8) ); // AA level

  LoadKeymap();
  App->EnableKeyRepeat(false);

  gui = new GUI(*App);

  objectmanager = new ObjectManager;

  networkhandler = new NetworkHandler;

  backdrop = new Backdrop(*App);

  bRunning = true;
}

Game::~Game() {
  bRunning = false;

  if(player) {
    delete player;
  }

  if( backdrop ) {
    delete backdrop;
  }

  if( networkhandler ) {
    delete networkhandler;
  }

  if( objectmanager ) {
    delete objectmanager;
  }

  if( gui ) {
    delete gui;
  }

  if(App) {
    delete App;
  }
}

Game* Game::GetGame() {
  if( !instance ) {
    instance = new Game();
  }
  return instance;
}

void Game::Run() {
  while ( bRunning && App->IsOpened() ) {
    float ElapsedTime = App->GetFrameTime();
    Tick(ElapsedTime);
    sf::Sleep(0.02 - ElapsedTime); // Limit 50 FPS
  }
}

void Game::Stop() {
  bRunning = false;
}

GUI* Game::GetGUI() {
  if( gui ) {
    return gui;
  } else {
    return NULL;
  }
}

Player* Game::GetPlayer() {
  if( player ) {
    return player;
  } else {
    return NULL;
  }
}

NetworkHandler* Game::GetNetworkHandler() {
  if( networkhandler ) {
    return networkhandler;
  } else {
    return NULL;
  }
}

ObjectManager* Game::GetObjectManager() {
  if( objectmanager ) {
    return objectmanager;
  } else {
    return NULL;
  }
}

Backdrop* Game::GetBackdrop() {
  if( backdrop ) {
    return backdrop;
  } else {
    return NULL;
  }
}

void Game::Tick(float t) {

  if( HandleEvents(*App) ) {
    bRunning = false;
  }

  networkhandler->Tick();

  // Clear the screen (fill it with black color)
  App->Clear(sf::Color(0, 0, 0));

  gui->Draw(*App);

  if( networkhandler->IsAuthenticated() ) {
    objectmanager->Tick(t);
    //partSys.Update(t);

    backdrop->Draw(*App);

    objectmanager->DrawAll(*App);
    //App.Draw(partSys);
  }

  // Display window contents on screen
    App->Display();
}
