#include "symb_func.h"
#include <stdlib.h>

SymbolFunc *new_symbol_func(char *name) {
  SymbolFunc *func = (SymbolFunc *)malloc(sizeof(SymbolFunc));
  func->name = name;
  field_list_init(&func->args);
  return func;
}

void arg_add(SymbolFunc *func, SymbolField *field) {
  field_list_add(&func->args, field);
}

void arg_cmp(SymbolFunc *func, FieldList *fl) {
  field_list_cmp(&func->args, fl);
}

void func_table_add(FuncTable *ft, SymbolFunc *func) {
  hash_map_put(ft, func->name, func);
}

SymbolFunc *func_table_lookup(FuncTable *ft, char *name) {
  return (SymbolFunc *)hash_map_get(ft, name);
}