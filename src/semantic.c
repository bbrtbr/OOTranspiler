#include "semantic.h"

static void copy_ast_method_to_method(Method *dest, AstMethod *src)
{
    int i;

    memset(dest, 0, sizeof(Method));
    strncpy(dest->name, src->name, MAX_NAME - 1);
    dest->name[MAX_NAME - 1] = '\0';
    dest->param_count = src->param_count;
    dest->body_count = src->body_count;

    for (i = 0; i < src->param_count; i++)
    {
        if (!is_primitive_type(src->params[i].type))
            die("parametro precisa ter tipo int, float ou bool");
        dest->params[i] = src->params[i];
    }

    for (i = 0; i < src->body_count; i++)
        dest->body[i] = src->body[i];
}

static void analyze_field(ClassInfo *cls, AstField *field)
{
    Field *target;

    if (!is_primitive_type(field->type))
        die("campo precisa ter tipo int, float ou bool");

    if (find_field(cls, field->name))
        die("campo ja existe na classe ou foi herdado");

    if (cls->field_count >= MAX_FIELDS)
        die("limite de campos excedido");

    target = &cls->fields[cls->field_count++];
    strncpy(target->type, field->type, MAX_NAME - 1);
    target->type[MAX_NAME - 1] = '\0';
    strncpy(target->name, field->name, MAX_NAME - 1);
    target->name[MAX_NAME - 1] = '\0';
}

static void analyze_method(ClassInfo *cls, AstMethod *method)
{
    Method *target = find_or_create_method(cls, method->name);
    copy_ast_method_to_method(target, method);
}

static void analyze_class(AstClass *ast_cls)
{
    ClassInfo *cls;
    int i;

    if (class_count >= MAX_CLASSES)
        die("limite de classes excedido");

    cls = &classes[class_count];
    memset(cls, 0, sizeof(ClassInfo));
    strncpy(cls->name, ast_cls->name, MAX_NAME - 1);
    cls->name[MAX_NAME - 1] = '\0';

    if (ast_cls->parent_name[0])
    {
        ClassInfo *parent = find_class(ast_cls->parent_name);
        if (!parent)
            die("classe pai precisa ser declarada antes da filha");
        inherit_from_parent(cls, parent);
    }

    if (ast_cls->has_constructor)
    {
        copy_ast_method_to_method(&cls->constructor, &ast_cls->constructor);
        cls->has_constructor = 1;
    }

    for (i = 0; i < ast_cls->field_count; i++)
        analyze_field(cls, &ast_cls->fields[i]);

    for (i = 0; i < ast_cls->method_count; i++)
        analyze_method(cls, &ast_cls->methods[i]);

    class_count++;
}

void semantic_analyze(AstProgram *program)
{
    int i;

    class_count = 0;
    object_count = 0;

    for (i = 0; i < program->class_count; i++)
        analyze_class(&program->classes[i]);
}
