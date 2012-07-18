#include <csignal>

#include <config.hpp>
#include <game.hpp>

void handle_interrupt( int /*param*/ ) {
	std::cout << std::endl << "Caught SIGINT, stopping..." << std::endl;
	Game::GetGame()->Stop();
}

int main( int /*argc*/, char** /*argv*/ ) {
	signal( SIGINT, handle_interrupt );

	return Game::GetGame()->Run();
}
