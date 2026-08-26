#include "ast.h"

AstProgram *ast_create_program(void)
{
    AstProgram *program = (AstProgram *)calloc(1, sizeof(AstProgram));
    if (!program)
        die("sem memoria para AST");
    return program;
}

void ast_free_program(AstProgram *program)
{
    free(program);
}
