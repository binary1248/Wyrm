#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "player.h"
#include "events.h"
#include "network.h"
#include "gui.h"
#include "objects/objects.h"
#include "objectmanager.h"
#include "backdrop.h"
#include "particle/particles.h"

bool bRunning = true;

int main(int argc, char** argv) {
  // Create the main rendering window
  sf::RenderWindow App(sf::VideoMode(1024, 768, 32), "WYRM");

  App.EnableKeyRepeat(false);

  LoadGUI(App);

  sf::String username("TestUser");
  sf::String password("TestPassword");


  /*
	//So we whant to have a nice scale in effect on our particle system
	//Lets create a scale in affector
	CScaleInAffector scale_in(0.1f, 0.f, 1.f);
	//Create a instance of the emitter we just made
	CMyEmitter emit;
	emit.SetColorRandom();
	emit.SetEmissionRate(0.02f, 10);
	emit.SetMaxParticles(500);
	emit.SetLiveTime(0.8f, 0.4f);
	emit.SetInitialDimensions(12.f, 12.f);
	emit.SetRotationSpeed(M_PI*100);
	emit.AddAffector(&scale_in);
	emit.Init();

	//Let's copy our newly created emitter
	CMyEmitter emit2(emit);
	emit2.SetOffset(sf::Vector2f(80.f, 0.f));
	//when we copy the new system will share the same particles as the one it copied from, so to give it it's own pool of particles call Init() again
	emit2.Init();

	//Finally we create a particle system with our emitters :)
	part::CParticleSystem partSys;
	partSys.AddEmitter(&emit);
	partSys.AddEmitter(&emit2);
	partSys.SetPosition(sf::Vector2f(400, 300));
  */

  while ( bRunning && App.IsOpened() && !networkhandler.authenticated ) {
    float ElapsedTime = App.GetFrameTime();
    if( HandleEvents(App) )
      bRunning = false;
    App.Clear(sf::Color(0, 0, 0));
    DrawGUI(App);
    App.Display();
    sf::Sleep(0.02 - ElapsedTime); // Limit 50 FPS
  }

  std::cout << "Proceeding to main game loop." << std::endl;

  LoadBackdrop(App);
  Image.LoadFromFile("spaceship.png");
  std::cout << Image.GetHeight() << std::endl;

  while ( bRunning && App.IsOpened() ) {
    float ElapsedTime = App.GetFrameTime();
    if( HandleEvents(App) )
      bRunning = false;

    objectmanager.Tick(ElapsedTime);
    //partSys.Update(ElapsedTime);

    // Clear the screen (fill it with black color)
    App.Clear(sf::Color(0, 0, 0));

    DrawBackdrop(App);

    objectmanager.DrawAll(App);
    //App.Draw(partSys);

    // Display window contents on screen
    App.Display();
    sf::Sleep(0.02 - ElapsedTime); // Limit 50 FPS
  }

  if(player) {
    delete player;
    player = 0;
  }

  networkhandler.Disconnect();
  networkhandler.Wait();

  return EXIT_SUCCESS;
}
