#ifndef __SYMB_FIELD_H__
#define __SYMB_FIELD_H__

#include "symb_type.h"

typedef struct SymbolField {
  LinkedStub stub;
  SymbolType *type;
  char *name;
} SymbolField;

SymbolField *new_symbol_field(SymbolType *, char *);

typedef HashMap FieldTable;

void field_table_add(FieldTable *, SymbolField *);
SymbolField *field_table_lookup(FieldTable *, char *);

#define field_table_init hash_map_init
#define field_table_free hash_map_free

typedef LinkedList FieldList;

void field_list_add(FieldList *, SymbolField *);
SymbolField *field_list_lookup(FieldList *, char *);
u1 field_list_cmp(FieldList *, FieldList *);

#define field_list_init linked_list_init
#define field_list_free linked_list_free

#endif