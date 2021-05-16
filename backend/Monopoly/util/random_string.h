#ifndef _RANDOM_STR_H_
#define _RANDOM_STR_H_

#include <string>

static const char alphanum[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";

void RandomString(char* str, unsigned int n);

void RandomString(std::string& str, unsigned int n);

#endif