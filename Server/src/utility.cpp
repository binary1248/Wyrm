#include <sstream>
#include <iostream>

#include <SFML/System.hpp>

#include "utility.h"

sf::Clock sfclock;

const std::string GetTimeString() {
  float time = static_cast<float>( sfclock.GetElapsedTime() ) / 1000.0f;
  std::stringstream ss;
  ss.setf( std::ios::fixed, std::ios::floatfield );
  ss.fill('0');
  ss.width(9);
  ss.precision(3);
  ss << time;
  return "[" + ss.str() + "]";
}

float GetTime() {
  return sfclock.GetElapsedTime();
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
