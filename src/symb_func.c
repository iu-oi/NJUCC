#include "symb_func.h"
#include <stdlib.h>
#include <string.h>

SymbolFunc *new_symbol_func(char *name) {
  SymbolFunc *func = (SymbolFunc *)malloc(sizeof(SymbolFunc));
  func->name = name;
  field_list_init(&func->paraml);
  return func;
}

char *_local2glob(char *sup, char *inf) {
  u4 sup_len = strlen(sup), inf_len = strlen(inf);
  char *tmp = (char *)malloc(sup_len + inf_len + 1);

  strcpy(tmp, sup);
  strcpy(tmp + sup_len, "$");
  strcpy(tmp + sup_len + 1, inf);

  char *full = new_string(tmp);
  free(tmp);
  return full;
}

void new_param(SymbolFunc *func, SymbolField *formal, FieldTable *paramt) {
  SymbolField *param =
      new_symbol_field(formal->type, _local2glob(func->name, formal->name));
  field_list_add(&func->paraml, param);
  field_table_add(paramt, param);
}

void func_table_add(FuncTable *ft, SymbolFunc *func) {
  hash_map_put(ft, func->name, func);
}

SymbolFunc *func_table_lookup(FuncTable *ft, char *name) {
  return (SymbolFunc *)hash_map_get(ft, name);
}