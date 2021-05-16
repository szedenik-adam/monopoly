#include "random_string.h"
#include <stdlib.h>

void RandomString(char* str, unsigned int n)
{
	while(n--) {
		*str++ = ( alphanum[rand() % (sizeof(alphanum) - 1)] );
	}
	*str = '\0';
}

void RandomString(std::string& str, unsigned int n)
{
	str.clear();

	for (int i = 0; i < 10; i++) {
		str.push_back( alphanum[rand() % (sizeof(alphanum) - 1)] );
	}
}
