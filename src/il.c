#include "il.h"
#include <stdlib.h>

Code *new_code(u4 op, ...) {
  va_list(args);

  Code *new_code = (Code *)malloc(sizeof(Code));
  new_code->stub.prev = new_code->stub.next = NULL;
  new_code->lineno = 0;
  new_code->op = op;
  new_code->dest = new_code->src1.var = new_code->src2.var = NULL;

  va_start(args, op);
  if (op <= OP_WRITE) {
    new_code->src1.var = va_arg(args, Variable *);
  } else if (op <= OP_GOTO) {
    new_code->src1.imm = va_arg(args, u4);
  } else if (op <= OP_MASSIGN) {
    new_code->dest = va_arg(args, Variable *);
    new_code->src1.var = va_arg(args, Variable *);
  } else if (op <= OP_ASSIGNI) {
    new_code->dest = va_arg(args, Variable *);
    new_code->src1.imm = va_arg(args, u4);
  } else if (op <= OP_DIV) {
    new_code->dest = va_arg(args, Variable *);
    new_code->src1.var = va_arg(args, Variable *);
    new_code->src2.var = va_arg(args, Variable *);
  } else if (op <= OP_DIVI) {
    new_code->dest = va_arg(args, Variable *);
    new_code->src1.var = va_arg(args, Variable *);
    new_code->src2.imm = va_arg(args, u4);
  } else if (op <= OP_IDIV) {
    new_code->dest = va_arg(args, Variable *);
    new_code->src1.imm = va_arg(args, u4);
    new_code->src2.var = va_arg(args, Variable *);
  } else if (op <= OP_LESS) {
    new_code->dest = va_arg(args, Variable *);
    new_code->src1.var = va_arg(args, Variable *);
    new_code->src2.var = va_arg(args, Variable *);
  } else {
    new_code->dest = va_arg(args, Variable *);
    new_code->src1.var = va_arg(args, Variable *);
    new_code->src2.imm = va_arg(args, u4);
  }
  va_end(args);

  return new_code;
}

void _show_code(Code *code, FILE *out) {
  const char *code_repr[] = {
      "\nfunc %s",
      " param %s",
      " arg %s",
      " ret %s",
      " read %s",
      " write %s",

      " arg %d",
      " ret %d",
      " write %d",
      " goto (%d)",

      " %s = call %s",
      " %s = %s",
      " %s = &%s",
      " %s = *%s",
      " *%s = %s",

      " glob %s %d",
      " local %s %d",
      " %s = %d",

      " %s = %s + %s",
      " %s = %s - %s",
      " %s = %s * %s",
      " %s = %s / %s",

      " %s = %s + %d",
      " %s = %s - %d",
      " %s = %s * %d",
      " %s = %s / %d",

      " %s = %d - %s",
      " %s = %d / %s",

      " if %s == %s goto (%d)",
      " if %s != %s goto (%d)",
      " if %s >= %s goto (%d)",
      " if %s <= %s goto (%d)",
      " if %s > %s goto (%d)",
      " if %s < %s goto (%d)",

      " if %s == %d goto (%d)",
      " if %s != %d goto (%d)",
      " if %s >= %d goto (%d)",
      " if %s <= %d goto (%d)",
      " if %s > %d goto (%d)",
      " if %s < %d goto (%d)",
  };

  const char *repr = code_repr[code->op - OP_FUNC];

  if (code->op <= OP_GOTO)
    fprintf(out, repr, code->src1.wild);
  else if (code->op <= OP_ASSIGNI)
    fprintf(out, repr, code->dest, code->src1.wild);
  else
    fprintf(out, repr, code->dest, code->src1.wild, code->src2.wild);

  fprintf(out, "\n");
  fflush(stdout);
}

void code_list_init(CodeList *cl) {
  hash_map_init(&cl->vars, hash_elf, 4096);
  linked_list_init(&cl->list);
  cl->local_ctr = cl->tmp_ctr = 0;
}

void code_list_free(CodeList *cl) {
  hash_map_free(&cl->vars);
  linked_list_free(&cl->list);
}

void code_list_show(CodeList *cl, FILE *out) {
  LinkedStub *stub = &cl->list.head;
  while ((stub = stub->next) != NULL)
    _show_code((Code *)stub, out);
}

void code_list_append(CodeList *cl, Code *code) {
  if (linked_list_empty(&cl->list))
    code->lineno = 1;
  else
    code->lineno = ((Code *)cl->list.tail)->lineno + 1;

  linked_list_push(&cl->list, &code->stub);
}

char *_num2name(char *fmt, u4 num) {
  char buf[16];
  sprintf(buf, fmt, num);
  return new_string(buf);
}

Variable *gen_glob(CodeList *cl, char *name, u4 mem) {
  Variable *glob_var = (Variable *)malloc(sizeof(Variable));
  glob_var->name = name;
  glob_var->mem = mem;

  hash_map_put(&cl->vars, name, glob_var);
  return glob_var;
}

Variable *gen_local(CodeList *cl, u4 mem) {
  Variable *local_var = (Variable *)malloc(sizeof(Variable));
  local_var->name = _num2name("v%d", cl->local_ctr++);
  local_var->mem = mem;

  hash_map_put(&cl->vars, local_var->name, local_var);
  return local_var;
}

Variable *gen_tmp(CodeList *cl) {
  Variable *tmp_var = (Variable *)malloc(sizeof(Variable));
  tmp_var->name = _num2name("t%d", cl->tmp_ctr++);
  tmp_var->mem = 1;

  hash_map_put(&cl->vars, tmp_var->name, tmp_var);
  return tmp_var;
}