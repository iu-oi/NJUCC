#ifndef __SYMB_FUNC_H__
#define __SYMB_FUNC_H__

#include "symb_field.h"

typedef struct SymbolFunc {
  char *name;
  FieldList paraml;
} SymbolFunc;

SymbolFunc *new_symbol_func(char *);

char *_local2glob(char *, char *);

void new_param(SymbolFunc *, SymbolField *, FieldTable *);

typedef HashMap FuncTable;

void func_table_add(FuncTable *, SymbolFunc *);
SymbolFunc *func_table_lookup(FuncTable *, char *);

#define func_table_init hash_map_init
#define func_table_free hash_map_free

#endif