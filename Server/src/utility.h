#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

#include <string>

#include <boost/lexical_cast.hpp>

#define STRING_CAST boost::lexical_cast<std::string>

const std::string GetTimeString();
float GetTime();

void LogConsole( const std::string& message );
void LogStartup( const std::string& message );
void Die( const std::string& message );

#endif // UTILITY_H_INCLUDED
