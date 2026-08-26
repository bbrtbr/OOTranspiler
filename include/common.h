#ifndef OO2LAB11_COMMON_H
#define OO2LAB11_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS          12000
#define MAX_TOKEN_TEXT      64
#define MAX_CLASSES         32
#define MAX_FIELDS          64
#define MAX_METHODS         64
#define MAX_METHOD_TOKENS   1000
#define MAX_OBJECTS         128
#define MAX_NAME            64
#define MAX_PARAMS          16
#define MAX_ARG_TOKENS      200

typedef struct
{
    char text[MAX_TOKEN_TEXT];
    int line;
} Token;

typedef struct
{
    char type[MAX_NAME];
    char name[MAX_NAME];
} Field;

typedef struct
{
    char type[MAX_NAME];
    char name[MAX_NAME];
} Param;

typedef struct
{
    char name[MAX_NAME];
    Param params[MAX_PARAMS];
    int param_count;
    Token body[MAX_METHOD_TOKENS];
    int body_count;
} Method;

typedef struct
{
    char name[MAX_NAME];
    char parent_name[MAX_NAME];
    Field fields[MAX_FIELDS];
    int field_count;
    Method methods[MAX_METHODS];
    int method_count;
    Method constructor;
    int has_constructor;
} ClassInfo;

typedef struct
{
    char name[MAX_NAME];
    char class_name[MAX_NAME];
    char dynamic_class_name[MAX_NAME];
} ObjectInfo;

typedef struct
{
    Token *tokens;
    int count;
    int pos;
} Stream;

struct Context;

typedef struct
{
    char name[MAX_NAME];
    Token tokens[MAX_ARG_TOKENS];
    int token_count;
    struct Context *caller_ctx;
} ParamBinding;

typedef struct Context
{
    char object_name[MAX_NAME];
    ClassInfo *class_info;
    ParamBinding bindings[MAX_PARAMS];
    int binding_count;
} Context;

void die_line(int line, const char *msg);
void die(const char *msg);

int is_identifier_start(char c);
int is_identifier_part(char c);
int is_identifier_text(const char *s);
int is_primitive_type(const char *s);
int is_keyword_kept(const char *s);

char *read_entire_file(const char *path);

int stream_has(Stream *s);
Token *peek(Stream *s);
int match(Stream *s, const char *text);
void expect(Stream *s, const char *text);
void expect_identifier(Stream *s, char *dest);

#endif






