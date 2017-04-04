
#include <stdarg.h>
#include <Debug.h>

#ifdef DEBUG
static const char* levelToString(debug_level_t level)
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
        case DEBUGGING:
            return "DEBUG";
		default:
			return "undefined";
	}
}
#endif

void DEBUG_LOG(debug_level_t level, const char* function, const char* format, ... )
{
	#ifdef DEBUG
    va_list args;
    va_start(args, format);
    
    printf("%s - %s: ", levelToString(level), function);
    vprintf(format, args);
    va_end(args);
    printf("\n");
	#endif
	return;
}
