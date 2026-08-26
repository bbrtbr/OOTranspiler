#include "transpiler.h"

#include "lexer.h"
#include "parser.h"
#include "semantic.h"
#include "codegen.h"

int TranspileFile(const char *input_path, const char *output_path)
{
    char *source;
    Stream stream;
    AstProgram *program;

    out = stdout;
    if (output_path)
    {
        out = fopen(output_path, "w");
        if (!out)
        {
            fprintf(stderr, "Erro: nao foi possivel criar arquivo de saida\n");
            return 1;
        }
    }

    source = read_entire_file(input_path);
    tokenize(source);
    free(source);

    stream.tokens = tokens;
    stream.count = token_count;
    stream.pos = 0;

    program = parse_program_ast(&stream);
    semantic_analyze(program);
    codegen_generate_lab11(program);
    ast_free_program(program);

    if (out != stdout)
        fclose(out);

    return 0;
}
