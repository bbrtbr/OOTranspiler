#ifndef OO2LAB11_LEXER_H
#define OO2LAB11_LEXER_H

#include "common.h"

extern Token tokens[MAX_TOKENS];
extern int token_count;

void tokenize(const char *src);

#endif
