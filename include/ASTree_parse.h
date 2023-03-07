#ifndef __ASTREE_PARSE_H__
#define __ASTREE_PARSE_H__

#include "ASTree.h"
#include "symb_field.h"
#include "symb_func.h"
#include "symb_type.h"

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

#define SDT_PARAM ASTNode *node, SymbolTable *table
#define SDT(LABEL) sdt_##LABEL

void SDT(program)(SDT_PARAM);
void SDT(ext_def_list)(SDT_PARAM);
void SDT(ext_def)(SDT_PARAM);
void SDT(def_list)(SDT_PARAM);
void SDT(def)(SDT_PARAM);
void SDT(dec_list)(SDT_PARAM);
SymbolField *SDT(dec)(SDT_PARAM, SymbolType *);
void SDT(fun_dec)(SDT_PARAM);
void SDT(var_list)(SDT_PARAM, SymbolFunc *);
void SDT(comp_st)(SDT_PARAM);
void SDT(stmt_list)(SDT_PARAM);
void SDT(stmt)(SDT_PARAM);
SymbolType *SDT(exp)(SDT_PARAM);
void SDT(args)(SDT_PARAM, SymbolField *);

#endif