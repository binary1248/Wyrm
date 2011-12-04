#ifndef UTILITY_HPP_INCLUDED
#define UTILITY_HPP_INCLUDED

#undef M_PI
#define M_PI 3.14159265358979323846f

#include <string>
#include <sstream>

const std::string GetTimeString();
unsigned int GetTime();

void LogConsole( const std::string& message );
void LogStartup( const std::string& message );
void Die( const std::string& message );

template <typename T>
inline std::string string_cast( T value );

inline float clean_angle( float angle );

#include <utility.inl>

#endif // UTILITY_HPP_INCLUDED
