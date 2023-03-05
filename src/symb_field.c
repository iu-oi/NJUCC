#include "symb_field.h"
#include <stdlib.h>
#include <string.h>

SymbolField *new_symbol_field(SymbolType *type, char *name) {
  SymbolField *new_field = (SymbolField *)malloc(sizeof(SymbolField));
  new_field->stub.prev = new_field->stub.next = NULL;
  new_field->type = type;
  new_field->name = name;

  return new_field;
}

void field_table_add(FieldTable *ft, SymbolField *field) {
  hash_map_put(ft, field->name, field);
}

SymbolField *field_table_lookup(FieldTable *ft, char *name) {
  return (SymbolField *)hash_map_get(ft, name);
}

void field_list_add(FieldList *ft, SymbolField *field) {
  linked_list_push(ft, &field->stub);
}

SymbolField *field_list_lookup(FieldList *fl, char *name) {
  LinkedStub *stub = fl->tail;

  while (stub != &fl->head) {
    if (strcmp(((SymbolField *)stub)->name, name) == 0)
      return (SymbolField *)stub;
    stub = stub->prev;
  }

  return NULL;
}