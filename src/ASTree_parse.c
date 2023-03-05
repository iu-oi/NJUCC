#include "ASTree_parse.h"
#include "stdlib.h"

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

FieldTable *current_frame(FrameStack *locals) {
  return (FieldTable *)frame_stack_top(locals);
}

void symbol_table_init(SymbolTable *table) {
  type_tree_init(&table->types);
  func_table_init(&table->funcs, hash_elf, 4096);

  field_table_init(&table->globs, hash_elf, 4096);
  frame_stack_init(&table->locals, 128);
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

void SDT(program)(SDT_ARGS) { sdt_ext_def_list(get_node(node, 1), table); }

void SDT(ext_def_list)(SDT_ARGS) {
  if (NON_TOKEN(node).ruleno == 1) {
    sdt_ext_def(get_node(node, 1), table);
    sdt_ext_def_list(get_node(node, 2), table);
  }
}

void SDT(ext_def)(SDT_ARGS) {
  if (NON_TOKEN(node).ruleno == 1) {
    sdt_dec_list(get_node(node, 2), table);
  } else {
    begin_frame(&table->locals);
    sdt_fun_dec(get_node(node, 2), table);
    sdt_comp_st(get_node(node, 3), table);
    end_frame(&table->locals);
  }
}

void SDT(def_list)(SDT_ARGS) {
  if (NON_TOKEN(node).ruleno == 1) {
    sdt_def(get_node(node, 1), table);
    sdt_def_list(get_node(node, 2), table);
  }
}

void SDT(def)(SDT_ARGS) { sdt_dec_list(get_node(node, 2), table); }

void SDT(dec_list)(SDT_ARGS) {
  SymbolField *new_field = sdt_dec(get_node(node, 1), table, BASIC_TYPE);

  FieldTable *frame;
  if (table->locals.size == 0)
    frame = &table->globs;
  else
    frame = current_frame(&table->locals);

  if (field_table_lookup(frame, new_field->name) != NULL) {
    /* todo error */
    free(new_field);
  } else {
    field_table_add(frame, new_field);
  }

  if (NON_TOKEN(node).ruleno == 2)
    sdt_dec_list(get_node(node, 3), table);
}

SymbolField *SDT(dec)(SDT_ARGS, SymbolType *type) {
  if (NON_TOKEN(node).ruleno == 1)
    return new_symbol_field(type, TOKEN(get_node(node, 1)).text);

  u4 len = (u4)strtol(TOKEN(get_node(node, 3)).text, NULL, 0);
  return sdt_dec(get_node(node, 1), table, new_symbol_type(type, len));
}

void SDT(fun_dec)(SDT_ARGS) {
  SymbolFunc *new_func = new_symbol_func(TOKEN(get_node(node, 1)).text);

  if (NON_TOKEN(node).ruleno == 1)
    sdt_var_list(get_node(node, 3), table, &new_func->argl);

  if (func_table_lookup(&table->funcs, new_func->name) != NULL) {
    /* todo error */
    free(new_func);
  } else {
    func_table_add(&table->funcs, new_func);
  }
}

void SDT(var_list)(SDT_ARGS, FieldList *argl) {
  SymbolField *new_field = sdt_dec(get_node(node, 2), table, BASIC_TYPE);

  FieldTable *frame = current_frame(&table->locals);
  if (field_table_lookup(frame, new_field->name) != NULL) {
    /* todo error */
    free(new_field);
  } else {
    field_table_add(frame, new_field);
    field_list_add(argl, new_field);
  }

  if (NON_TOKEN(node).ruleno == 1)
    sdt_var_list(get_node(node, 4), table, argl);
}

void SDT(comp_st)(SDT_ARGS) {
  sdt_def_list(get_node(node, 2), table);
  sdt_stmt_list(get_node(node, 3), table);
}

void SDT(stmt_list)(SDT_ARGS) {
  if (NON_TOKEN(node).ruleno == 1) {
    sdt_stmt(get_node(node, 1), table);
    sdt_stmt_list(get_node(node, 2), table);
  }
}

void SDT(stmt)(SDT_ARGS) {
  u4 ruleno = NON_TOKEN(node).ruleno;

  if (ruleno == 1) {
    sdt_exp(get_node(node, 1), table);
  } else if (ruleno == 2) {
    begin_frame(&table->locals);
    sdt_comp_st(get_node(node, 1), table);
    end_frame(&table->locals);
  } else if (ruleno == 3) {
    SymbolType *ret_type = sdt_exp(get_node(node, 2), table);
    if (ret_type != BASIC_TYPE) {
      /* todo error */
    }
  } else if (ruleno == 4) {
    sdt_exp(get_node(node, 3), table);
    sdt_stmt(get_node(node, 5), table);
  } else if (ruleno == 5) {
    sdt_exp(get_node(node, 3), table);
    sdt_stmt(get_node(node, 5), table);
    sdt_stmt(get_node(node, 7), table);
  } else {
    sdt_exp(get_node(node, 3), table);
    sdt_stmt(get_node(node, 5), table);
  }
}

SymbolType *SDT(exp)(SDT_ARGS) {
  u4 ruleno = NON_TOKEN(node).ruleno;

  if (ruleno == 1) {
    u4 left_rule_no = NON_TOKEN(get_node(node, 1)).ruleno;
    SymbolType *type_left = sdt_exp(get_node(node, 1), table);
    SymbolType *type_right = sdt_exp(get_node(node, 3), table);
    if (left_rule_no != 16 && left_rule_no != 17) {
      /* error here */
    }
    if (type_left != BASIC_TYPE || type_left != BASIC_TYPE) {
      /* error here */
    }
  } else if (ruleno >= 2 && ruleno <= 8) {
    SymbolType *type_arg1 = sdt_exp(get_node(node, 1), table);
    SymbolType *type_arg2 = sdt_exp(get_node(node, 3), table);
    if (type_arg1 != BASIC_TYPE || type_arg2 != BASIC_TYPE) {
      /* error here */
    }
  } else if (ruleno == 9) {
    return sdt_exp(get_node(node, 2), table);
  } else if (ruleno == 10 || ruleno == 11) {
    SymbolType *type_arg1 = sdt_exp(get_node(node, 2), table);
    if (type_arg1 != BASIC_TYPE) {
      /* error here */
    }
  } else if (ruleno == 12) {
    SymbolType *type_arg1 = sdt_exp(get_node(node, 3), table);
    if (type_arg1 != BASIC_TYPE) {
      /* error here */
    }
  } else if (ruleno == 13) {
    char *tag = TOKEN(get_node(node, 1)).text;
    SymbolFunc *func = func_table_lookup(&table->funcs, tag);
    if (func == NULL) {
      /* error here */
    }
    sdt_args(get_node(node, 3), table, (SymbolField *)func->argl.head.next);
  } else if (ruleno == 14) {
  } else if (ruleno == 15) {
    char *tag = TOKEN(get_node(node, 1)).text;
    SymbolFunc *func = func_table_lookup(&table->funcs, tag);
    if (func == NULL) {
      /* error here */
    }
  } else if (ruleno == 16) {
    SymbolType *type_arr = sdt_exp(get_node(node, 1), table);
    SymbolType *type_index = sdt_exp(get_node(node, 3), table);
    if (type_arr == BASIC_TYPE) {
      /* error here */
    }
    if (type_index != BASIC_TYPE) {
      /* error here */
    }
    return type_arr->subtype;
  } else if (ruleno == 17) {
    char *tag = TOKEN(get_node(node, 1)).text;
    SymbolField *field = symbol_table_lookup(table, tag);
    if (field == NULL) {
      /* error here */
    }
    return field->type;
  } else if (ruleno == 18) {
  }

  return BASIC_TYPE;
}

void SDT(args)(SDT_ARGS, SymbolField *formal) {
  SymbolType *curr_type = sdt_exp(get_node(node, 1), table);
  if (formal != NULL && curr_type != formal->type) {
    /* error here */
  }

  if (NON_TOKEN(node).ruleno == 1) {
    if (formal == NULL)
      sdt_args(get_node(node, 3), table, NULL);
    else
      sdt_args(get_node(node, 3), table, (SymbolField *)formal->stub.next);
  }
}