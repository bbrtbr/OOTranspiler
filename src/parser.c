#include "parser.h"

static void parse_param_list(Stream *s, AstMethod *m)
{
    char type[MAX_NAME];
    char name[MAX_NAME];

    expect(s, "(");

    if (match(s, ")"))
        return;

    while (stream_has(s))
    {
        if (m->param_count >= MAX_PARAMS)
            die("limite de parametros excedido");

        strncpy(type, peek(s)->text, MAX_NAME - 1);
        type[MAX_NAME - 1] = '\0';
        s->pos++;

        expect_identifier(s, name);

        strncpy(m->params[m->param_count].type, type, MAX_NAME - 1);
        m->params[m->param_count].type[MAX_NAME - 1] = '\0';
        strncpy(m->params[m->param_count].name, name, MAX_NAME - 1);
        m->params[m->param_count].name[MAX_NAME - 1] = '\0';
        m->param_count++;

        if (match(s, ","))
            continue;

        expect(s, ")");
        break;
    }
}

static void copy_method_body(Stream *s, AstMethod *m)
{
    int depth;

    expect(s, "{");
    depth = 1;

    while (stream_has(s) && depth > 0)
    {
        Token tk = s->tokens[s->pos++];

        if (strcmp(tk.text, "{") == 0)
        {
            depth++;
        }
        else if (strcmp(tk.text, "}") == 0)
        {
            depth--;
            if (depth == 0)
                break;
        }

        if (m->body_count >= MAX_METHOD_TOKENS)
            die_line(tk.line, "corpo de metodo muito grande");

        m->body[m->body_count++] = tk;
    }
}

static void parse_constructor(Stream *s, AstClass *cls)
{
    AstMethod *ctor = &cls->constructor;

    if (cls->has_constructor)
        die_line(peek(s)->line, "apenas um construtor e suportado");

    memset(ctor, 0, sizeof(AstMethod));
    strncpy(ctor->name, cls->name, MAX_NAME - 1);
    ctor->name[MAX_NAME - 1] = '\0';
    cls->has_constructor = 1;

    s->pos++;
    parse_param_list(s, ctor);
    copy_method_body(s, ctor);
}

static void parse_method(Stream *s, AstClass *cls, const char *name)
{
    AstMethod *m;

    if (cls->method_count >= MAX_METHODS)
        die("limite de metodos excedido");

    m = &cls->methods[cls->method_count++];
    memset(m, 0, sizeof(AstMethod));
    strncpy(m->name, name, MAX_NAME - 1);
    m->name[MAX_NAME - 1] = '\0';

    parse_param_list(s, m);
    copy_method_body(s, m);
}

static void parse_field(Stream *s, AstClass *cls, const char *type, const char *name)
{
    AstField *f;

    if (cls->field_count >= MAX_FIELDS)
        die("limite de campos excedido");

    f = &cls->fields[cls->field_count++];
    strncpy(f->type, type, MAX_NAME - 1);
    f->type[MAX_NAME - 1] = '\0';
    strncpy(f->name, name, MAX_NAME - 1);
    f->name[MAX_NAME - 1] = '\0';

    expect(s, ";");
}

static void parse_class(Stream *s, AstProgram *program)
{
    AstClass *cls;
    char type[MAX_NAME];
    char name[MAX_NAME];

    if (program->class_count >= MAX_CLASSES)
        die("limite de classes excedido");

    expect(s, "class");
    cls = &program->classes[program->class_count++];
    memset(cls, 0, sizeof(AstClass));

    expect_identifier(s, cls->name);

    if (match(s, ":"))
        expect_identifier(s, cls->parent_name);

    expect(s, "{");

    while (stream_has(s) && strcmp(peek(s)->text, "}") != 0)
    {
        if ((strcmp(peek(s)->text, "public") == 0 ||
             strcmp(peek(s)->text, "private") == 0 ||
             strcmp(peek(s)->text, "protected") == 0) &&
            s->pos + 1 < s->count &&
            strcmp(s->tokens[s->pos + 1].text, ":") == 0)
        {
            s->pos += 2;
            continue;
        }

        if (strcmp(peek(s)->text, cls->name) == 0 &&
            s->pos + 1 < s->count &&
            strcmp(s->tokens[s->pos + 1].text, "(") == 0)
        {
            parse_constructor(s, cls);
            continue;
        }

        strncpy(type, peek(s)->text, MAX_NAME - 1);
        type[MAX_NAME - 1] = '\0';
        s->pos++;

        expect_identifier(s, name);

        if (strcmp(type, "void") == 0 && strcmp(peek(s)->text, "(") == 0)
            parse_method(s, cls, name);
        else
            parse_field(s, cls, type, name);
    }

    expect(s, "}");
    match(s, ";");
}

static void copy_main_block(Stream *s, AstProgram *program)
{
    int depth;

    expect(s, "{");
    depth = 1;

    if (program->main_body_count >= MAX_TOKENS)
        die("bloco main muito grande");
    program->main_body[program->main_body_count++] = (Token){"{", peek(s)->line};

    while (stream_has(s) && depth > 0)
    {
        Token tk = s->tokens[s->pos++];

        if (strcmp(tk.text, "{") == 0)
        {
            depth++;
        }
        else if (strcmp(tk.text, "}") == 0)
        {
            depth--;
        }

        if (program->main_body_count >= MAX_TOKENS)
            die_line(tk.line, "bloco main muito grande");

        program->main_body[program->main_body_count++] = tk;

        if (depth == 0)
            break;
    }
}

AstProgram *parse_program_ast(Stream *s)
{
    AstProgram *program = ast_create_program();

    while (stream_has(s) && strcmp(peek(s)->text, "class") == 0)
        parse_class(s, program);

    expect(s, "int");
    expect(s, "main");
    expect(s, "(");
    expect(s, ")");

    copy_main_block(s, program);
    return program;
}
