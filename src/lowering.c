#include "lowering_internal.h"

static void lower_declaration(Stream *s, int indent)
{
    char type[MAX_NAME];
    char name[MAX_NAME];
    char final_name[MAX_NAME];

    strncpy(type, peek(s)->text, MAX_NAME - 1);
    type[MAX_NAME - 1] = '\0';
    s->pos++;

    expect_identifier(s, name);
    sanitize_identifier(name, final_name);

    lower_print_indent(indent);
    fprintf(out, "%s %s", type, final_name);

    if (match(s, "["))
    {
        fprintf(out, "[ ");
        lower_print_expression_until(s, "]", NULL);
        expect(s, "]");
        fprintf(out, "]");
    }

    expect(s, ";");
    fprintf(out, ";\n");
}

static void lower_simple_statement(Stream *s, Context *ctx, int indent)
{
    lower_print_indent(indent);
    lower_print_expression_until(s, ";", ctx);
    expect(s, ";");
    fprintf(out, ";\n");
}

static void lower_if_or_while(Stream *s, Context *ctx, int indent)
{
    char keyword[MAX_NAME];

    strncpy(keyword, peek(s)->text, MAX_NAME - 1);
    keyword[MAX_NAME - 1] = '\0';
    s->pos++;

    lower_print_indent(indent);
    fprintf(out, "%s ", keyword);
    lower_print_parenthesized_condition(s, ctx);
    fprintf(out, "\n");

    lower_item(s, ctx, indent);
}

static void lower_do_while(Stream *s, Context *ctx, int indent)
{
    s->pos++;

    lower_print_indent(indent);
    fprintf(out, "do\n");
    lower_item(s, ctx, indent);

    if (!match(s, "while"))
        die_line(peek(s)->line, "esperado while depois de do");

    lower_print_indent(indent);
    fprintf(out, "while ");
    lower_print_parenthesized_condition(s, ctx);
    expect(s, ";");
    fprintf(out, ";\n");
}

static void skip_return_statement(Stream *s)
{
    while (stream_has(s) && !match(s, ";"))
        s->pos++;
}

void lower_block(Stream *s, Context *ctx, int indent)
{
    expect(s, "{");
    lower_print_indent(indent);
    fprintf(out, "{\n");

    while (stream_has(s) && strcmp(peek(s)->text, "}") != 0)
        lower_item(s, ctx, indent + 1);

    expect(s, "}");
    lower_print_indent(indent);
    fprintf(out, "}\n");
}

void lower_item(Stream *s, Context *ctx, int indent)
{
    ClassInfo *cls;

    if (!stream_has(s))
        return;

    if (strcmp(peek(s)->text, "return") == 0)
    {
        skip_return_statement(s);
        return;
    }

    if (strcmp(peek(s)->text, "else") == 0)
        die_line(peek(s)->line, "else ainda nao e suportado pelo Lab11");

    if (strcmp(peek(s)->text, "{") == 0)
    {
        lower_block(s, ctx, indent);
        return;
    }

    if (strcmp(peek(s)->text, "if") == 0 || strcmp(peek(s)->text, "while") == 0)
    {
        lower_if_or_while(s, ctx, indent);
        return;
    }

    if (strcmp(peek(s)->text, "do") == 0)
    {
        lower_do_while(s, ctx, indent);
        return;
    }

    if (is_primitive_type(peek(s)->text))
    {
        lower_declaration(s, indent);
        return;
    }

    cls = find_class(peek(s)->text);
    if (cls && s->pos + 2 < s->count &&
        (strcmp(s->tokens[s->pos + 2].text, ";") == 0 || strcmp(s->tokens[s->pos + 2].text, "(") == 0))
    {
        lower_object_declaration(s, cls, ctx, indent);
        return;
    }

    if (lower_is_object_assignment_statement(s))
    {
        lower_object_assignment_statement(s, indent);
        return;
    }

    if (lower_is_method_call_statement(s))
    {
        lower_method_call_statement(s, ctx, indent);
        return;
    }

    lower_simple_statement(s, ctx, indent);
}
