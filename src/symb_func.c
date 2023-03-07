#include "symb_func.h"
#include <stdlib.h>
#include <string.h>

SymbolFunc *new_symbol_func(char *name) {
  SymbolFunc *func = (SymbolFunc *)malloc(sizeof(SymbolFunc));
  func->name = name;
  field_list_init(&func->paraml);
  return func;
}

void new_param(char *func_name, char *param_name, SymbolType *param_type,
               FieldList *paraml, FieldTable *paramt) {
  char *spec_name = (char *)malloc(strlen(func_name) + strlen(param_name) + 2);
  strcpy(spec_name, func_name);
  strcat(spec_name, "$");
  strcat(spec_name, param_name);

  SymbolField *param = new_symbol_field(param_type, new_string(spec_name));
  field_list_add(paraml, param);
  field_table_add(paramt, param);

  free(spec_name);
}

void func_table_add(FuncTable *ft, SymbolFunc *func) {
  hash_map_put(ft, func->name, func);
}

SymbolFunc *func_table_lookup(FuncTable *ft, char *name) {
  return (SymbolFunc *)hash_map_get(ft, name);
}