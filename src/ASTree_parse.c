#include "ASTree_parse.h"
#include "njucc.h"
#include <stdlib.h>

void SDT(program)(SDT_PARAM) { sdt_ext_def_list(get_node(node, 1), table); }

void SDT(ext_def_list)(SDT_PARAM) {
  if (NON_TOKEN(node).ruleno == 1) {
    sdt_ext_def(get_node(node, 1), table);
    sdt_ext_def_list(get_node(node, 2), table);
  }
}

void SDT(ext_def)(SDT_PARAM) {
  if (NON_TOKEN(node).ruleno == 1) {
    sdt_dec_list(get_node(node, 2), table);
  } else {
    begin_frame(&table->locals);
    sdt_fun_dec(get_node(node, 2), table);
    sdt_comp_st(get_node(node, 3), table);
    end_frame(&table->locals);
  }
}

void SDT(def_list)(SDT_PARAM) {
  if (NON_TOKEN(node).ruleno == 1) {
    sdt_def(get_node(node, 1), table);
    sdt_def_list(get_node(node, 2), table);
  }
}

void SDT(def)(SDT_PARAM) { sdt_dec_list(get_node(node, 2), table); }

void SDT(dec_list)(SDT_PARAM) {
  SymbolField *new_field = sdt_dec(get_node(node, 1), table, BASIC_TYPE);
  FieldTable *frame = current_frame(table);

  if (field_table_lookup(frame, new_field->name) != NULL) {
    semantic_error("duplicated variable", NON_TOKEN(node).lineno);
    free(new_field);
  } else {
    field_table_add(frame, new_field);
  }

  if (NON_TOKEN(node).ruleno == 2)
    sdt_dec_list(get_node(node, 3), table);
}

SymbolField *SDT(dec)(SDT_PARAM, SymbolType *type) {
  if (NON_TOKEN(node).ruleno == 1)
    return new_symbol_field(type, TOKEN(get_node(node, 1)).text);

  u4 len = (u4)strtol(TOKEN(get_node(node, 3)).text, NULL, 0);
  return sdt_dec(get_node(node, 1), table, new_symbol_type(type, len));
}

void SDT(fun_dec)(SDT_PARAM) {
  SymbolFunc *new_func = new_symbol_func(TOKEN(get_node(node, 1)).text);

  if (NON_TOKEN(node).ruleno == 2)
    sdt_var_list(get_node(node, 3), table, new_func);

  if (func_table_lookup(&table->funcs, new_func->name) != NULL) {
    semantic_error("duplicated function", NON_TOKEN(node).lineno);
    free(new_func);
  } else {
    func_table_add(&table->funcs, new_func);
  }
}

void SDT(var_list)(SDT_PARAM, SymbolFunc *func) {
  SymbolField *new_field = sdt_dec(get_node(node, 2), table, BASIC_TYPE);
  FieldTable *frame = current_frame(table);

  if (field_table_lookup(frame, new_field->name) != NULL) {
    semantic_error("duplicated parameter", NON_TOKEN(node).lineno);
    free(new_field);
  } else {
    field_table_add(frame, new_field);
    new_param(func, new_field, &table->globs);
  }

  if (NON_TOKEN(node).ruleno == 2)
    sdt_var_list(get_node(node, 4), table, func);
}

void SDT(comp_st)(SDT_PARAM) {
  sdt_def_list(get_node(node, 2), table);
  sdt_stmt_list(get_node(node, 3), table);
}

void SDT(stmt_list)(SDT_PARAM) {
  if (NON_TOKEN(node).ruleno == 1) {
    sdt_stmt(get_node(node, 1), table);
    sdt_stmt_list(get_node(node, 2), table);
  }
}

void SDT(stmt)(SDT_PARAM) {
  u4 ruleno = NON_TOKEN(node).ruleno;

  if (ruleno == 1) {
    sdt_exp(get_node(node, 1), table);
  } else if (ruleno == 2) {
    begin_frame(&table->locals);
    sdt_comp_st(get_node(node, 1), table);
    end_frame(&table->locals);
  } else if (ruleno == 3) {
    SymbolType *ret_type = sdt_exp(get_node(node, 2), table);
    if (ret_type != BASIC_TYPE)
      semantic_error("returning a non-basic type", NON_TOKEN(node).lineno);
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

SymbolType *SDT(exp)(SDT_PARAM) {
  u4 ruleno = NON_TOKEN(node).ruleno;

  if (ruleno == 1) {
    return sdt_exp(get_node(node, 2), table);

  } else if (ruleno == 2) {
    u4 left_rule_no = NON_TOKEN(get_node(node, 1)).ruleno;
    SymbolType *type_left = sdt_exp(get_node(node, 1), table);
    SymbolType *type_right = sdt_exp(get_node(node, 3), table);

    if (left_rule_no != 16 && left_rule_no != 17)
      semantic_error("right-value assignment", NON_TOKEN(node).lineno);
    else if (type_left != BASIC_TYPE || type_left != BASIC_TYPE)
      semantic_error("assigning non-basic value(s)", NON_TOKEN(node).lineno);

  } else if (ruleno >= 3 && ruleno <= 6) {
    SymbolType *type_arg1 = sdt_exp(get_node(node, 1), table);
    SymbolType *type_arg2 = sdt_exp(get_node(node, 3), table);

    if (type_arg1 != BASIC_TYPE || type_arg2 != BASIC_TYPE)
      semantic_error("operating non-basic value(s)", NON_TOKEN(node).lineno);

  } else if (ruleno == 7) {
    SymbolType *type_arg1 = sdt_exp(get_node(node, 2), table);

    if (type_arg1 != BASIC_TYPE)
      semantic_error("operating non-basic value", NON_TOKEN(node).lineno);

  } else if (ruleno == 8) {
    SymbolType *type_arg1 = sdt_exp(get_node(node, 2), table);

    if (type_arg1 != BASIC_TYPE)
      semantic_error("non-basic boolean value", NON_TOKEN(node).lineno);

  } else if (ruleno >= 9 && ruleno <= 11) {
    SymbolType *type_arg1 = sdt_exp(get_node(node, 1), table);
    SymbolType *type_arg2 = sdt_exp(get_node(node, 3), table);

    if (type_arg1 != BASIC_TYPE || type_arg2 != BASIC_TYPE)
      semantic_error("non-basic boolean value(s)", NON_TOKEN(node).lineno);

  } else if (ruleno == 12) {
  } else if (ruleno == 13) {
    SymbolType *type_arg1 = sdt_exp(get_node(node, 3), table);

    if (type_arg1 != BASIC_TYPE)
      semantic_error("writing a non-basic value", NON_TOKEN(node).lineno);
  } else if (ruleno == 14 || ruleno == 15) {
    char *tag = TOKEN(get_node(node, 1)).text;
    SymbolFunc *func = func_table_lookup(&table->funcs, tag);

    if (func == NULL)
      semantic_error("undefined function", NON_TOKEN(node).lineno);
    else if (ruleno == 15)
      sdt_args(get_node(node, 3), table,
               (SymbolField *)linked_list_first(&func->paraml));

  } else if (ruleno == 16) {
    SymbolType *type_arr = sdt_exp(get_node(node, 1), table);
    SymbolType *type_index = sdt_exp(get_node(node, 3), table);

    if (type_arr == BASIC_TYPE)
      semantic_error("dereferencing a basic value", NON_TOKEN(node).lineno);
    else if (type_index != BASIC_TYPE)
      semantic_error("non-basic index", NON_TOKEN(node).lineno);
    else
      return type_arr->subtype;

  } else if (ruleno == 17) {
    char *tag = TOKEN(get_node(node, 1)).text;
    SymbolField *field = symbol_table_lookup(table, tag);

    if (field == NULL)
      semantic_error("undefined variable", NON_TOKEN(node).lineno);
    else
      return field->type;

  } else if (ruleno == 18) {
  }

  return BASIC_TYPE;
}

void SDT(args)(SDT_PARAM, SymbolField *param) {
  SymbolType *curr_type = sdt_exp(get_node(node, 1), table);
  if (param != NULL && curr_type != param->type)
    semantic_error("argument mismatch", NON_TOKEN(node).lineno);

  if (NON_TOKEN(node).ruleno == 2) {
    if (param == NULL)
      sdt_args(get_node(node, 3), table, NULL);
    else
      sdt_args(get_node(node, 3), table, (SymbolField *)param->stub.next);
  }
}