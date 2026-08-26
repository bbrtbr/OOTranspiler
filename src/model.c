#include "model.h"

ClassInfo classes[MAX_CLASSES];
int class_count = 0;
ObjectInfo objects[MAX_OBJECTS];
int object_count = 0;
FILE *out = NULL;

ClassInfo *find_class(const char *name)
{
    int i;

    for (i = 0; i < class_count; i++)
    {
        if (strcmp(classes[i].name, name) == 0)
            return &classes[i];
    }

    return NULL;
}

Field *find_field(ClassInfo *cls, const char *name)
{
    int i;

    if (!cls)
        return NULL;

    for (i = 0; i < cls->field_count; i++)
    {
        if (strcmp(cls->fields[i].name, name) == 0)
            return &cls->fields[i];
    }

    return NULL;
}

Method *find_method(ClassInfo *cls, const char *name)
{
    int i;

    if (!cls)
        return NULL;

    for (i = 0; i < cls->method_count; i++)
    {
        if (strcmp(cls->methods[i].name, name) == 0)
            return &cls->methods[i];
    }

    return NULL;
}

ParamBinding *find_binding(Context *ctx, const char *name)
{
    int i;

    if (!ctx)
        return NULL;

    for (i = ctx->binding_count - 1; i >= 0; i--)
    {
        if (strcmp(ctx->bindings[i].name, name) == 0)
            return &ctx->bindings[i];
    }

    return NULL;
}

ObjectInfo *find_object(const char *name)
{
    int i;

    for (i = object_count - 1; i >= 0; i--)
    {
        if (strcmp(objects[i].name, name) == 0)
            return &objects[i];
    }

    return NULL;
}

void add_object(const char *name, const char *class_name)
{
    if (object_count >= MAX_OBJECTS)
        die("limite de objetos excedido");

    strncpy(objects[object_count].name, name, MAX_NAME - 1);
    objects[object_count].name[MAX_NAME - 1] = '\0';
    strncpy(objects[object_count].class_name, class_name, MAX_NAME - 1);
    objects[object_count].class_name[MAX_NAME - 1] = '\0';
    strncpy(objects[object_count].dynamic_class_name, class_name, MAX_NAME - 1);
    objects[object_count].dynamic_class_name[MAX_NAME - 1] = '\0';
    object_count++;
}

void sanitize_identifier(const char *src, char *dest)
{
    int i;
    int j = 0;

    for (i = 0; src[i] && j < MAX_NAME - 1; i++)
    {
        if (isalpha((unsigned char)src[i]))
            dest[j++] = src[i];
    }

    if (j == 0)
        dest[j++] = 'v';

    dest[j] = '\0';

    if (is_keyword_kept(dest))
        strncat(dest, "Var", MAX_NAME - strlen(dest) - 1);
}

void flattened_name(const char *object_name, const char *field_name, char *dest)
{
    char obj[MAX_NAME];
    char field[MAX_NAME];
    int len;

    sanitize_identifier(object_name, obj);
    sanitize_identifier(field_name, field);

    strncpy(dest, obj, MAX_NAME - 1);
    dest[MAX_NAME - 1] = '\0';

    len = (int)strlen(dest);
    if (len < MAX_NAME - 1 && field[0])
    {
        if (islower((unsigned char)field[0]))
            field[0] = (char)toupper((unsigned char)field[0]);
        strncat(dest, field, MAX_NAME - strlen(dest) - 1);
    }
}

int is_subclass(const char *child_name, const char *parent_name)
{
    ClassInfo *cls;

    if (strcmp(child_name, parent_name) == 0)
        return 1;

    cls = find_class(child_name);
    while (cls && cls->parent_name[0])
    {
        if (strcmp(cls->parent_name, parent_name) == 0)
            return 1;
        cls = find_class(cls->parent_name);
    }

    return 0;
}


void inherit_from_parent(ClassInfo *cls, ClassInfo *parent)
{
    int i;

    if (!cls || !parent)
        return;

    strncpy(cls->parent_name, parent->name, MAX_NAME - 1);
    cls->parent_name[MAX_NAME - 1] = '\0';

    for (i = 0; i < parent->field_count; i++)
    {
        if (cls->field_count >= MAX_FIELDS)
            die("limite de campos excedido durante heranca");
        cls->fields[cls->field_count++] = parent->fields[i];
    }

    for (i = 0; i < parent->method_count; i++)
    {
        if (cls->method_count >= MAX_METHODS)
            die("limite de metodos excedido durante heranca");
        cls->methods[cls->method_count++] = parent->methods[i];
    }
}

Method *find_or_create_method(ClassInfo *cls, const char *name)
{
    int i;
    Method *m;

    for (i = 0; i < cls->method_count; i++)
    {
        if (strcmp(cls->methods[i].name, name) == 0)
        {
            m = &cls->methods[i];
            memset(m, 0, sizeof(Method));
            strncpy(m->name, name, MAX_NAME - 1);
            m->name[MAX_NAME - 1] = '\0';
            return m;
        }
    }

    if (cls->method_count >= MAX_METHODS)
        die("limite de metodos excedido");

    m = &cls->methods[cls->method_count++];
    memset(m, 0, sizeof(Method));
    strncpy(m->name, name, MAX_NAME - 1);
    m->name[MAX_NAME - 1] = '\0';
    return m;
}


Method *vmt_lookup_method(ClassInfo *dynamic_cls, const char *method_name)
{
    /*
        VMT didatica: cada ClassInfo funciona como uma tabela de metodos.
        Como a heranca copia os metodos do pai e override substitui por nome,
        procurar o metodo na classe dinamica equivale a consultar a VMT.
    */
    return find_method(dynamic_cls, method_name);
}

