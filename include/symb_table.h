#ifndef __SYMB_TABLE_H__
#define __SYMB_TABLE_H__

#include "symb_func.h"

typedef ArrayStack FrameStack;

#define frame_stack_init array_stack_init
#define frame_stack_push array_stack_push
#define frame_stack_pop array_stack_pop
#define frame_stack_free array_stack_free
#define frame_at array_stack_get
#define frame_stack_empty array_stack_empty
#define frame_stack_top array_stack_top

void begin_frame(FrameStack *);
void end_frame(FrameStack *);

typedef struct SymbolTable {
  TypeTree types;
  FuncTable funcs;
  FieldTable globs;
  FrameStack locals;
} SymbolTable;

FieldTable *current_frame(SymbolTable *);

#define BASIC_TYPE ((SymbolType *)table->types.root)

void symbol_table_init(SymbolTable *);
void symbol_table_free(SymbolTable *);
SymbolField *symbol_table_lookup(SymbolTable *, char *);

#endif