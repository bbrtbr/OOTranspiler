#include "lowering_internal.h"

void lower_object_declaration(Stream *s, ClassInfo *cls, Context *ctx, int indent)
{
    char object_name[MAX_NAME];
    char flat[MAX_NAME];
    ParamBinding bindings[MAX_PARAMS];
    int binding_count = 0;
    int has_constructor_call = 0;
    int i;

    s->pos++;
    expect_identifier(s, object_name);

    if (strcmp(peek(s)->text, "(") == 0)
    {
        if (!cls->has_constructor)
            die_line(peek(s)->line, "classe nao tem construtor declarado");
        has_constructor_call = 1;
        lower_parse_argument_bindings(s, &cls->constructor, bindings, &binding_count, ctx);
    }
    else if (cls->has_constructor && cls->constructor.param_count > 0)
    {
        die_line(peek(s)->line, "construtor precisa de argumentos");
    }

    expect(s, ";");
    add_object(object_name, cls->name);

    for (i = 0; i < cls->field_count; i++)
    {
        flattened_name(object_name, cls->fields[i].name, flat);
        lower_print_indent(indent);
        fprintf(out, "%s %s;\n", cls->fields[i].type, flat);
    }

    if (cls->has_constructor && (has_constructor_call || cls->constructor.param_count == 0))
        lower_inline_method(&cls->constructor, object_name, cls, indent, bindings, binding_count);
}

int lower_is_object_assignment_statement(Stream *s)
{
    return s->pos + 3 < s->count &&
           is_identifier_text(s->tokens[s->pos].text) &&
           strcmp(s->tokens[s->pos + 1].text, "=") == 0 &&
           is_identifier_text(s->tokens[s->pos + 2].text) &&
           strcmp(s->tokens[s->pos + 3].text, ";") == 0 &&
           find_object(s->tokens[s->pos].text) &&
           find_object(s->tokens[s->pos + 2].text);
}

void lower_object_assignment_statement(Stream *s, int indent)
{
    ObjectInfo *target = find_object(s->tokens[s->pos].text);
    ObjectInfo *source = find_object(s->tokens[s->pos + 2].text);
    ClassInfo *target_cls = find_class(target->class_name);
    ClassInfo *source_cls = find_class(source->class_name);
    char left[MAX_NAME];
    char right[MAX_NAME];
    int i;

    if (!is_subclass(source_cls->name, target_cls->name))
        die_line(s->tokens[s->pos].line, "atribuicao de objeto exige mesmo tipo ou classe filha");

    for (i = 0; i < target_cls->field_count; i++)
    {
        flattened_name(target->name, target_cls->fields[i].name, left);
        flattened_name(source->name, target_cls->fields[i].name, right);
        lower_print_indent(indent);
        fprintf(out, "%s = %s;\n", left, right);
    }

    strncpy(target->dynamic_class_name, source->dynamic_class_name, MAX_NAME - 1);
    target->dynamic_class_name[MAX_NAME - 1] = '\0';

    s->pos += 4;
}
