#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 5
#endif

#ifndef LOG_LEVEL
#define LOG_LEVEL 0
#endif

#ifndef LOG_OUTPUT
#define LOG_OUTPUT stdout
#endif

#ifdef DEBUG_LOGGING
#define DEBUG_LOG(level, ...) do { if(level <= DEBUG_LEVEL) { timestamp(LOG_OUTPUT); fprintf(LOG_OUTPUT, __VA_ARGS__); } } while(0);
#else
#define DEBUG_LOG(level, ...) do { } while (0);
#endif

#ifdef STANDARD_LOGGING
#define LOG(level, ...) do { if(level <= LOG_LEVEL) { timestamp(LOG_OUTPUT); fprintf(LOG_OUTPUT, __VA_ARGS__); } } while(0);
#else
#define LOG(level, ...) do { } while(0);
#endif

void timestamp(FILE* output);

