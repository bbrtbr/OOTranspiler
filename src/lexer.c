#include "lexer.h"

Token tokens[MAX_TOKENS];
int token_count = 0;

static void add_token(const char *text, int line)
{
    if (token_count >= MAX_TOKENS)
        die("limite de tokens excedido");

    strncpy(tokens[token_count].text, text, MAX_TOKEN_TEXT - 1);
    tokens[token_count].text[MAX_TOKEN_TEXT - 1] = '\0';
    tokens[token_count].line = line;
    token_count++;
}

void tokenize(const char *src)
{
    int i = 0;
    int line = 1;

    while (src[i])
    {
        char c = src[i];

        if (c == '\n')
        {
            line++;
            i++;
            continue;
        }

        if (isspace((unsigned char)c))
        {
            i++;
            continue;
        }

        if (c == '/' && src[i + 1] == '/')
        {
            i += 2;
            while (src[i] && src[i] != '\n')
                i++;
            continue;
        }

        if (c == '/' && src[i + 1] == '*')
        {
            i += 2;
            while (src[i] && !(src[i] == '*' && src[i + 1] == '/'))
            {
                if (src[i] == '\n')
                    line++;
                i++;
            }
            if (src[i])
                i += 2;
            continue;
        }

        if (is_identifier_start(c))
        {
            char text[MAX_TOKEN_TEXT];
            int j = 0;

            while (is_identifier_part(src[i]) && j < MAX_TOKEN_TEXT - 1)
                text[j++] = src[i++];
            text[j] = '\0';

            while (is_identifier_part(src[i]))
                i++;

            add_token(text, line);
            continue;
        }

        if (isdigit((unsigned char)c))
        {
            char text[MAX_TOKEN_TEXT];
            int j = 0;

            while ((isdigit((unsigned char)src[i]) || src[i] == '.') && j < MAX_TOKEN_TEXT - 1)
                text[j++] = src[i++];
            text[j] = '\0';

            while (isdigit((unsigned char)src[i]) || src[i] == '.')
                i++;

            add_token(text, line);
            continue;
        }

        if ((c == '&' && src[i + 1] == '&') ||
            (c == '|' && src[i + 1] == '|') ||
            (c == '=' && src[i + 1] == '=') ||
            (c == '!' && src[i + 1] == '=') ||
            (c == '<' && src[i + 1] == '=') ||
            (c == '>' && src[i + 1] == '='))
        {
            char text[3];
            text[0] = c;
            text[1] = src[i + 1];
            text[2] = '\0';
            add_token(text, line);
            i += 2;
            continue;
        }

        {
            char text[2];
            text[0] = c;
            text[1] = '\0';
            add_token(text, line);
            i++;
        }
    }
}
