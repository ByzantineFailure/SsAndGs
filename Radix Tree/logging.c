#include "logging.h"

void timestamp(FILE* output)
{
	time_t stamptime;
	stamptime = time(NULL);
	fprintf(output, "%s  ", asctime(localtime(&stamptime))); 
}