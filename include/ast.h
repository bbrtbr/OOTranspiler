#ifndef OO2LAB11_AST_H
#define OO2LAB11_AST_H

#include "common.h"

typedef struct
{
    char type[MAX_NAME];
    char name[MAX_NAME];
} AstField;

typedef struct
{
    char name[MAX_NAME];
    Param params[MAX_PARAMS];
    int param_count;
    Token body[MAX_METHOD_TOKENS];
    int body_count;
} AstMethod;

typedef struct
{
    char name[MAX_NAME];
    char parent_name[MAX_NAME];
    AstField fields[MAX_FIELDS];
    int field_count;
    AstMethod methods[MAX_METHODS];
    int method_count;
    AstMethod constructor;
    int has_constructor;
} AstClass;

typedef struct
{
    AstClass classes[MAX_CLASSES];
    int class_count;
    Token main_body[MAX_TOKENS];
    int main_body_count;
} AstProgram;

AstProgram *ast_create_program(void);
void ast_free_program(AstProgram *program);

#endif
