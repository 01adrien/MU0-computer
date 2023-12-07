#ifndef _TRANSLATOR_H_
#define _TRANSLATOR_H_

#include "../../../const.h"
#include "scanner.h"

#define ASM_MAX_LINE 128

char*
parse_all(scanner_t* scanner);

void
parse_line(scanner_t* scanner);

#endif
