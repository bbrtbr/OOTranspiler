#ifndef OO2LAB11_LOWERING_INTERNAL_H
#define OO2LAB11_LOWERING_INTERNAL_H

#include "lowering.h"

void lower_print_indent(int indent);
void lower_print_expression_until(Stream *s, const char *end, Context *ctx);
void lower_print_parenthesized_condition(Stream *s, Context *ctx);

void lower_parse_argument_bindings(Stream *s, Method *method, ParamBinding *bindings, int *binding_count, Context *caller_ctx);
void lower_inline_method(Method *method, const char *object_name, ClassInfo *cls, int indent, ParamBinding *bindings, int binding_count);

int lower_is_method_call_statement(Stream *s);
void lower_method_call_statement(Stream *s, Context *ctx, int indent);

int lower_is_object_assignment_statement(Stream *s);
void lower_object_assignment_statement(Stream *s, int indent);
void lower_object_declaration(Stream *s, ClassInfo *cls, Context *ctx, int indent);

#endif
