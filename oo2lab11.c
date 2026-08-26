/*
    Arquivo de compatibilidade.

    O transpilador foi dividido na pasta src/.
    Para compilar o projeto, use o makefile:

        mingw32-make

    Ou compile diretamente:

        gcc -Wall -Wextra -std=c99 src/main.c src/common.c src/lexer.c src/ast.c src/visitor.c src/model.c src/semantic.c src/lowering_emit.c src/lowering_methods.c src/lowering_objects.c src/lowering.c src/codegen.c src/parser.c -o oo2lab11
*/

#include "src/common.c"
#include "src/lexer.c"
#include "src/ast.c"
#include "src/visitor.c"
#include "src/model.c"
#include "src/semantic.c"
#include "src/lowering_emit.c"
#include "src/lowering_methods.c"
#include "src/lowering_objects.c"
#include "src/lowering.c"
#include "src/codegen.c"
#include "src/parser.c"
#include "src/main.c"

