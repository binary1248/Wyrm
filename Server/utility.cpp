#include <sstream>
#include <iostream>
#include <SFML/System.hpp>
#include "utility.h"

sf::Clock sfclock;

std::string GetTimeString() {
  float time = (float)(sfclock.GetElapsedTime()) / 1000.0f;
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

void LogConsole(std::string s) {
  std::cout << GetTimeString() << " " << s << std::endl;
}
