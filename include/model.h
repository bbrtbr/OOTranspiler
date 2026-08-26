#ifndef OO2LAB11_MODEL_H
#define OO2LAB11_MODEL_H

#include "common.h"

extern ClassInfo classes[MAX_CLASSES];
extern int class_count;
extern ObjectInfo objects[MAX_OBJECTS];
extern int object_count;
extern FILE *out;

ClassInfo *find_class(const char *name);
Field *find_field(ClassInfo *cls, const char *name);
Method *find_method(ClassInfo *cls, const char *name);
ParamBinding *find_binding(Context *ctx, const char *name);
ObjectInfo *find_object(const char *name);
void add_object(const char *name, const char *class_name);
int is_subclass(const char *child_name, const char *parent_name);
void inherit_from_parent(ClassInfo *cls, ClassInfo *parent);
Method *find_or_create_method(ClassInfo *cls, const char *name);
Method *vmt_lookup_method(ClassInfo *dynamic_cls, const char *method_name);

void sanitize_identifier(const char *src, char *dest);
void flattened_name(const char *object_name, const char *field_name, char *dest);

#endif





