#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include "mm.h"

typedef struct SymbolType {
  BinaryTreeStub stub;
  struct SymbolType *subtype;
  u4 len;
  u4 mem;
} SymbolType;

SymbolType *new_symbol_type(SymbolType *subtype, u4 len);

typedef BinaryTree TypeTree;

void type_tree_init(TypeTree *tt);
#define type_tree_free binary_tree_free

typedef struct SymbolField {
  LinkedStub stub;
  SymbolType *type;
  char *name;
} SymbolField;

SymbolField *new_symbol_field(SymbolType *type, char *name);

typedef HashMap FieldTable;

#define field_table_init hash_map_init
#define field_table_free hash_map_free
#define field_table_add(FT, F) hash_map_put(FT, (F)->name, F)
#define field_table_lookup(FT, NAME) ((SymbolField *)hash_map_get(FT, NAME))

typedef LinkedList FieldList;

#define field_list_init linked_list_init
#define field_list_free linked_list_free
#define field_list_add(FL, F) linked_list_push(FL, &(F)->stub);
SymbolField *field_list_lookup(FieldList *fl, char *name);

typedef struct SymbolFunc {
  char *name;
  FieldList paraml;
} SymbolFunc;

SymbolFunc *new_symbol_func(char *name);

char *_local2glob(char *sup, char *inf);

void new_param(SymbolFunc *func, SymbolField *formal, FieldTable *paramt);

typedef HashMap FuncTable;

#define func_table_init hash_map_init
#define func_table_free hash_map_free
#define func_table_add(FT, FUNC) hash_map_put(FT, (FUNC)->name, FUNC)
#define func_table_lookup(FT, NAME) ((SymbolFunc *)hash_map_get(FT, NAME))

typedef ArrayStack FrameStack;

#define frame_stack_init array_stack_init
#define frame_stack_free array_stack_free
#define frame_stack_empty array_stack_empty
#define frame_at array_stack_get
#define frame_stack_push array_stack_push
#define frame_stack_pop array_stack_pop
#define frame_stack_top array_stack_top

void begin_frame(FrameStack *locals);
void end_frame(FrameStack *locals);

typedef struct SymbolTable {
  TypeTree types;
  FuncTable funcs;
  FieldTable globs;
  FrameStack locals;
} SymbolTable;

#define BASIC_TYPE ((SymbolType *)table->types.root)

FieldTable *current_frame(SymbolTable *table);

void symbol_table_init(SymbolTable *table);
void symbol_table_free(SymbolTable *table);
SymbolField *symbol_table_lookup(SymbolTable *table, char *name);

#endif