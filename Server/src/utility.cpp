#include <sstream>
#include <chrono>

#include <SFML/System.hpp>

#include <config.hpp>
#include <utility.hpp>

using namespace std::chrono;

time_point<high_resolution_clock> start_point = high_resolution_clock::now();

const std::string GetTimeString() {
	float time = static_cast<float>( duration_cast<microseconds>( high_resolution_clock::now() - start_point ).count() );
	std::stringstream ss;
	ss.setf( std::ios::fixed, std::ios::floatfield );
	ss.fill( '0' );
	ss.width( 14 );
	ss.precision( 6 );
	ss << time / 1000000.0f;
	return "[" + ss.str() + "]";
}

unsigned int GetTime() {
	return static_cast<unsigned int>( duration_cast<milliseconds>( high_resolution_clock::now() - start_point ).count() );
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
