#include "symb_table.h"
#include <stdlib.h>

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