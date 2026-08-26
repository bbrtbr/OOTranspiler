#include "common.h"

void die_line(int line, const char *msg)
{
    fprintf(stderr, "Erro na linha %d: %s\n", line, msg);
    exit(1);
}

void die(const char *msg)
{
    fprintf(stderr, "Erro: %s\n", msg);
    exit(1);
}

int is_identifier_start(char c)
{
    return isalpha((unsigned char)c) || c == '_';
}

int is_identifier_part(char c)
{
    return isalnum((unsigned char)c) || c == '_';
}

int is_identifier_text(const char *s)
{
    int i;

    if (!is_identifier_start(s[0]))
        return 0;

    for (i = 1; s[i]; i++)
    {
        if (!is_identifier_part(s[i]))
            return 0;
    }

    return 1;
}

int is_primitive_type(const char *s)
{
    return strcmp(s, "int") == 0 ||
           strcmp(s, "float") == 0 ||
           strcmp(s, "bool") == 0;
}

int is_keyword_kept(const char *s)
{
    return strcmp(s, "int") == 0 ||
           strcmp(s, "float") == 0 ||
           strcmp(s, "bool") == 0 ||
           strcmp(s, "true") == 0 ||
           strcmp(s, "false") == 0 ||
           strcmp(s, "if") == 0 ||
           strcmp(s, "while") == 0 ||
           strcmp(s, "do") == 0 ||
           strcmp(s, "main") == 0;
}

char *read_entire_file(const char *path)
{
    FILE *f;
    long size;
    char *buffer;

    f = fopen(path, "rb");
    if (!f)
        die("nao foi possivel abrir o arquivo de entrada");

    fseek(f, 0, SEEK_END);
    size = ftell(f);
    rewind(f);

    buffer = (char *)malloc((size_t)size + 1);
    if (!buffer)
        die("sem memoria");

    if (fread(buffer, 1, (size_t)size, f) != (size_t)size)
        die("falha ao ler arquivo");

    buffer[size] = '\0';
    fclose(f);
    return buffer;
}

int stream_has(Stream *s)
{
    return s->pos < s->count;
}

Token *peek(Stream *s)
{
    if (!stream_has(s))
        die("fim inesperado do arquivo");
    return &s->tokens[s->pos];
}

int match(Stream *s, const char *text)
{
    if (stream_has(s) && strcmp(s->tokens[s->pos].text, text) == 0)
    {
        s->pos++;
        return 1;
    }
    return 0;
}

void expect(Stream *s, const char *text)
{
    char msg[128];

    if (!stream_has(s))
        die("fim inesperado do arquivo");

    if (!match(s, text))
    {
        snprintf(msg, sizeof(msg), "esperado '%s', encontrado '%s'", text, peek(s)->text);
        die_line(peek(s)->line, msg);
    }
}

void expect_identifier(Stream *s, char *dest)
{
    Token *t = peek(s);

    if (!is_identifier_text(t->text))
        die_line(t->line, "identificador esperado");

    strncpy(dest, t->text, MAX_NAME - 1);
    dest[MAX_NAME - 1] = '\0';
    s->pos++;
}
