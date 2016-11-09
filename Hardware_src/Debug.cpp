
#include <iostream>
#include <Debug.h>

#ifdef DEBUG
static std::string levelToString(debug_level_t level)
{
	switch(level)
	{
		case CRITICAL:
			return "CRITICAL";
		case ERROR:
			return "ERROR";
		case WARNING:
			return "WARNING";
		case INFO:
			return "INFO";
		default:
			return "undefined";
	}
}
#endif

void DEBUG_LOG(debug_level_t level, std::string function, std::string msg)
{
	#ifdef DEBUG
	std::cout << levelToString(level) << " - " << function << ": " << msg << std::endl;
	#endif
	return;
}
