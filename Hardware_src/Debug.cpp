
#include <string>
#include <iostream>
#include <Debug.h>

using namespace std;

#ifdef DEBUG
static string levelToString(int level)
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

void DEBUG_LOG(int level, string function, string msg)
{
	#ifdef DEBUG
	cout << levelToString(level) << " - " << function << ": " << msg << endl;
	#endif
	return;
}