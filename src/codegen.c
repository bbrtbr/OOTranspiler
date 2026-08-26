#include "codegen.h"

void codegen_generate_lab11(AstProgram *program)
{
    Stream main_stream;
    Context empty_context;

    memset(&empty_context, 0, sizeof(Context));

    main_stream.tokens = program->main_body;
    main_stream.count = program->main_body_count;
    main_stream.pos = 0;

    fprintf(out, "int main()\n");
    lower_block(&main_stream, &empty_context, 0);
}
