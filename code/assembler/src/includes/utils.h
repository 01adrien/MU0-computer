#ifndef _UTILS_H_
#define _UTILS_H_

#include "common.h"
#include "lexer.h"

char*
int_to_bin(uint32_t num, int n);

char*
read_file(const char* path);

#endif // !