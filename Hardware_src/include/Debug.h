#ifndef DEBUG_H
#define DEBUG_H

#include <Types.h>

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

void DEBUG_LOG(debug_level_t level, std::string function, std::string msg);

#endif // DEBUG_H
