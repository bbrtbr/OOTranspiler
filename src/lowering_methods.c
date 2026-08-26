#include "lowering_internal.h"

void lower_inline_method(Method *method, const char *object_name, ClassInfo *cls, int indent, ParamBinding *bindings, int binding_count)
{
    Stream ms;
    Context mctx;
    int i;

    ms.tokens = method->body;
    ms.count = method->body_count;
    ms.pos = 0;

    memset(&mctx, 0, sizeof(Context));
    strncpy(mctx.object_name, object_name, MAX_NAME - 1);
    mctx.object_name[MAX_NAME - 1] = '\0';
    mctx.class_info = cls;
    mctx.binding_count = binding_count;

    for (i = 0; i < binding_count; i++)
        mctx.bindings[i] = bindings[i];

    while (stream_has(&ms))
        lower_item(&ms, &mctx, indent);
}

void lower_parse_argument_bindings(Stream *s, Method *method, ParamBinding *bindings, int *binding_count, Context *caller_ctx)
{
    int index = 0;

    *binding_count = 0;
    expect(s, "(");

    if (match(s, ")"))
    {
        if (method->param_count != 0)
            die("faltam argumentos na chamada");
        return;
    }

    while (stream_has(s))
    {
        int depth = 0;
        ParamBinding *b;

        if (index >= method->param_count)
            die_line(peek(s)->line, "argumentos demais na chamada");

        b = &bindings[index];
        memset(b, 0, sizeof(ParamBinding));
        strncpy(b->name, method->params[index].name, MAX_NAME - 1);
        b->name[MAX_NAME - 1] = '\0';
        b->caller_ctx = caller_ctx;

        while (stream_has(s))
        {
            Token tk = *peek(s);

            if (strcmp(tk.text, "(") == 0)
            {
                depth++;
            }
            else if (strcmp(tk.text, ")") == 0)
            {
                if (depth == 0)
                    break;
                depth--;
            }
            else if (strcmp(tk.text, ",") == 0 && depth == 0)
            {
                break;
            }

            if (b->token_count >= MAX_ARG_TOKENS)
                die_line(tk.line, "argumento muito grande");

            b->tokens[b->token_count++] = tk;
            s->pos++;
        }

        if (b->token_count == 0)
            die_line(peek(s)->line, "argumento vazio");

        index++;

        if (match(s, ","))
            continue;

        expect(s, ")");
        break;
    }

    if (index != method->param_count)
        die("quantidade errada de argumentos");

    *binding_count = index;
}

int lower_is_method_call_statement(Stream *s)
{
    return s->pos + 3 < s->count &&
           is_identifier_text(s->tokens[s->pos].text) &&
           strcmp(s->tokens[s->pos + 1].text, ".") == 0 &&
           is_identifier_text(s->tokens[s->pos + 2].text) &&
           strcmp(s->tokens[s->pos + 3].text, "(") == 0;
}

void lower_method_call_statement(Stream *s, Context *ctx, int indent)
{
    char object_name[MAX_NAME];
    char method_name[MAX_NAME];
    ClassInfo *declared_cls = NULL;
    ClassInfo *dynamic_cls = NULL;
    Method *method = NULL;
    ParamBinding bindings[MAX_PARAMS];
    int binding_count = 0;

    strncpy(object_name, s->tokens[s->pos].text, MAX_NAME - 1);
    object_name[MAX_NAME - 1] = '\0';
    strncpy(method_name, s->tokens[s->pos + 2].text, MAX_NAME - 1);
    method_name[MAX_NAME - 1] = '\0';

    if (strcmp(object_name, "this") == 0)
    {
        if (!ctx || !ctx->class_info)
            die_line(s->tokens[s->pos].line, "this usado fora de metodo");

        declared_cls = ctx->class_info;
        dynamic_cls = ctx->class_info;
        strncpy(object_name, ctx->object_name, MAX_NAME - 1);
        object_name[MAX_NAME - 1] = '\0';
    }
    else
    {
        ObjectInfo *obj = find_object(object_name);
        if (!obj)
            die_line(s->tokens[s->pos].line, "objeto usado antes da declaracao");

        declared_cls = find_class(obj->class_name);
        dynamic_cls = find_class(obj->dynamic_class_name);
    }

    method = find_method(declared_cls, method_name);
    if (!method)
        die_line(s->tokens[s->pos].line, "metodo nao encontrado");

    s->pos += 3;
    lower_parse_argument_bindings(s, method, bindings, &binding_count, ctx);
    expect(s, ";");

    method = vmt_lookup_method(dynamic_cls, method_name);
    if (!method)
        die_line(s->tokens[s->pos - 1].line, "metodo nao encontrado na VMT");

    lower_inline_method(method, object_name, dynamic_cls, indent, bindings, binding_count);
}
