#include "lowering_internal.h"

void lower_print_indent(int indent)
{
    int i;
    for (i = 0; i < indent; i++)
        fprintf(out, "  ");
}

static void lower_print_token(Stream *s, Context *ctx);

static void lower_print_binding_value(ParamBinding *binding)
{
    Stream bs;

    bs.tokens = binding->tokens;
    bs.count = binding->token_count;
    bs.pos = 0;

    fprintf(out, "( ");
    while (stream_has(&bs))
        lower_print_token(&bs, binding->caller_ctx);
    fprintf(out, ") ");
}

static void lower_print_token(Stream *s, Context *ctx)
{
    char name[MAX_NAME];
    Token *t = peek(s);
    ParamBinding *binding;

    if (s->pos + 2 < s->count &&
        strcmp(s->tokens[s->pos + 1].text, ".") == 0 &&
        is_identifier_text(s->tokens[s->pos + 2].text))
    {
        const char *object_name = t->text;
        const char *field_name = s->tokens[s->pos + 2].text;

        if (strcmp(object_name, "this") == 0)
        {
            if (!ctx || !ctx->class_info)
                die_line(t->line, "uso de this fora de metodo");
            flattened_name(ctx->object_name, field_name, name);
        }
        else
        {
            ObjectInfo *obj = find_object(object_name);
            if (!obj)
                die_line(t->line, "objeto nao declarado");
            flattened_name(obj->name, field_name, name);
        }

        fprintf(out, "%s ", name);
        s->pos += 3;
        return;
    }

    binding = find_binding(ctx, t->text);
    if (binding)
    {
        lower_print_binding_value(binding);
        s->pos++;
        return;
    }

    if (ctx && ctx->class_info && find_field(ctx->class_info, t->text))
    {
        flattened_name(ctx->object_name, t->text, name);
        fprintf(out, "%s ", name);
        s->pos++;
        return;
    }

    if (is_identifier_text(t->text) && !is_keyword_kept(t->text))
    {
        sanitize_identifier(t->text, name);
        fprintf(out, "%s ", name);
        s->pos++;
        return;
    }

    fprintf(out, "%s ", t->text);
    s->pos++;
}

void lower_print_expression_until(Stream *s, const char *end, Context *ctx)
{
    while (stream_has(s) && strcmp(peek(s)->text, end) != 0)
        lower_print_token(s, ctx);
}

void lower_print_parenthesized_condition(Stream *s, Context *ctx)
{
    int depth = 1;

    expect(s, "(");
    fprintf(out, "( ");

    while (stream_has(s) && depth > 0)
    {
        if (strcmp(peek(s)->text, "(") == 0)
        {
            depth++;
            fprintf(out, "( ");
            s->pos++;
        }
        else if (strcmp(peek(s)->text, ")") == 0)
        {
            depth--;
            if (depth == 0)
            {
                s->pos++;
                break;
            }
            fprintf(out, ") ");
            s->pos++;
        }
        else
        {
            lower_print_token(s, ctx);
        }
    }

    if (depth != 0)
        die("parenteses nao fechado");

    fprintf(out, ")");
}
