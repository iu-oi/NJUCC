#include "symb.h"
#include <stdlib.h>
#include <string.h>

SymbolType *new_symbol_type(SymbolType *subtype, u4 len) {
  BinaryTreeStub *stub = &subtype->stub;

  stub = stub->left;
  while (stub != NULL) {
    if (((SymbolType *)stub)->len == len)
      return (SymbolType *)stub;
    stub = stub->right;
  }

  SymbolType *new_type = (SymbolType *)malloc(sizeof(SymbolType));
  new_type->stub.left = new_type->stub.right = NULL;
  new_type->subtype = subtype;
  new_type->len = len;
  new_type->mem = subtype->mem * len;

  new_type->stub.right = subtype->stub.left;
  subtype->stub.left = &new_type->stub;

  return new_type;
}

void type_tree_init(TypeTree *tt) {
  SymbolType *basic_type = (SymbolType *)malloc(sizeof(SymbolType));
  basic_type->stub.left = basic_type->stub.right = NULL;
  basic_type->subtype = NULL;
  basic_type->len = basic_type->mem = 1;

  binary_tree_init(tt);
  tt->root = &basic_type->stub;
}

SymbolField *new_symbol_field(SymbolType *type, char *name) {
  SymbolField *new_field = (SymbolField *)malloc(sizeof(SymbolField));
  new_field->stub.prev = new_field->stub.next = NULL;
  new_field->type = type;
  new_field->name = name;

  return new_field;
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

void begin_frame(FrameStack *locals) {
  FieldTable *new_table = (FieldTable *)malloc(sizeof(FieldTable));
  field_table_init(new_table, hash_elf, 4096);
  frame_stack_push(locals, new_table);
}

void end_frame(FrameStack *locals) {
  FieldTable *local_table = (FieldTable *)frame_stack_pop(locals);
  field_table_free(local_table);
  free(local_table);
}

FieldTable *current_frame(SymbolTable *table) {
  if (frame_stack_empty(&table->locals))
    return &table->globs;

  return (FieldTable *)frame_stack_top(&table->locals);
}

void symbol_table_init(SymbolTable *table) {
  type_tree_init(&table->types);
  func_table_init(&table->funcs, hash_elf, LARGE_HSH_TBL);

  field_table_init(&table->globs, hash_elf, LARGE_HSH_TBL);
  frame_stack_init(&table->locals, SHORT_ARR_LST);
}

void symbol_table_free(SymbolTable *table) {
  type_tree_free(&table->types);
  func_table_free(&table->funcs);

  field_table_free(&table->globs);
  frame_stack_free(&table->locals);
}

SymbolField *symbol_table_lookup(SymbolTable *table, char *name) {
  u4 frame_no = table->locals.size;
  SymbolField *match = NULL;

  while (frame_no-- != 0) {
    FieldTable *frame = frame_at(&table->locals, frame_no);
    if ((match = field_table_lookup(frame, name)) != NULL)
      return match;
  }

  return field_table_lookup(&table->globs, name);
}