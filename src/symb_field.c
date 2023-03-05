#include "symb_field.h"
#include <stdlib.h>
#include <string.h>

SymbolField *new_symbol_field(SymbolType *type, char *name) {
  SymbolField *new_field = (SymbolField *)malloc(sizeof(SymbolField));
  new_field->stub.prev = new_field->stub.next = NULL;
  new_field->type = type;
  new_field->name = name;
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

u1 field_list_cmp(FieldList *fl1, FieldList *fl2) {
  LinkedStub *stub1 = fl1->tail, *stub2 = fl2->tail;

  while (stub1 != &fl1->head && stub2 != &fl2->head) {
    if (((SymbolField *)stub1)->type != ((SymbolField *)stub2)->type)
      return 1;
    else if (strcmp(((SymbolField *)stub1)->name,
                    ((SymbolField *)stub2)->name) != 0)
      return 1;
    stub1 = stub1->prev;
    stub2 = stub2->prev;
  }

  if (stub1 != &fl1->head || stub2 != &fl2->head)
    return 1;
  return 0;
}