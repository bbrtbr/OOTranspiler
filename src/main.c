#include <stdio.h>
#include "transpiler.h"

int main(int argc, char **argv)
{
    if (argc < 2 || argc > 3)
    {
        fprintf(stderr, "uso: oo2lab11 entrada.cpp [saida.prog]\n");
        return 1;
    }

    return TranspileFile(argv[1], argc == 3 ? argv[2] : NULL);
}
