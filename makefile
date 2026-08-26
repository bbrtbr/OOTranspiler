CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -Iinclude
SRC=src/main.c src/transpiler.c src/common.c src/lexer.c src/ast.c src/model.c src/semantic.c src/lowering_emit.c src/lowering_methods.c src/lowering_objects.c src/lowering.c src/codegen.c src/parser.c

all: oo2lab11

oo2lab11: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o oo2lab11

run: oo2lab11
	./oo2lab11 examples/pessoa_param.cpp examples/pessoa_param.lab11

clean:
	del /Q oo2lab11.exe 2>NUL
	del /Q examples\pessoa.lab11 2>NUL
	del /Q examples\pessoa_param.lab11 2>NUL
	del /Q examples\heranca_polimorfismo.lab11 2>NUL

