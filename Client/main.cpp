#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "player.h"
#include "network.h"
#include "gui.h"
#include "game.h"
#include "objects/objects.h"
#include "objectmanager.h"
#include "particle/particles.h"

int main(int argc, char** argv) {

  Game::GetGame()->Run();

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

  return EXIT_SUCCESS;
}
