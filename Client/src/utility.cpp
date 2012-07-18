#include <sstream>

#include <SFML/System.hpp>

#include <config.hpp>
#include <utility.hpp>

sf::Clock time_clock;

const std::string GetTimeString() {
	float time = static_cast<float>( time_clock.getElapsedTime().asMicroseconds() );
	std::stringstream ss;
	ss.setf( std::ios::fixed, std::ios::floatfield );
	ss.fill( '0' );
	ss.width( 14 );
	ss.precision( 6 );
	ss << time / 1000000.0f;
	return "[" + ss.str() + "]";
}

unsigned int GetTime() {
	return static_cast<unsigned int>( time_clock.getElapsedTime().asMilliseconds() );
}

void LogConsole( const std::string& message ) {
	std::cout << GetTimeString() << " " << message << std::endl;
}

void LogStartup( const std::string& message ) {
	std::cout << message << std::endl;
}

void Die( const std::string& message ) {
	std::cout << message << std::endl;
	exit( EXIT_FAILURE );
}
