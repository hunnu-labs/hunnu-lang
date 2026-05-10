#ifndef HUNNU_TRANSPILE_H
#define HUNNU_TRANSPILE_H

#include "ast/ast.h"
#include <stdio.h>

char* transpile_to_c(ASTNode* program);

#endif
