#ifndef DEBUG_H
#define DEBUG_H

#include <string>
using namespace std;

/**
 * @file Debug.h
 * @brief A collection of functions and constants used for debugging.
 * @author jessicamorris 
 */
 
/**
 * @fn void DEBUG_LOG(int level, string function, string msg)
 * @brief Log a debug message to the command prompt.
 * 
 * @param[in] level The level of the message (CRITICAL, ERROR, etc.)
 * @param[in] function The name of the function (__FUNCTION__)
 * @param[in] msg The message to log
 */

// Copying this from Python because I like it
#define CRITICAL 50
#define ERROR 40
#define WARNING 30
#define INFO 20

void DEBUG_LOG(int level, string function, string msg);

#endif // DEBUG_H
