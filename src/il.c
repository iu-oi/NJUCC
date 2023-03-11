#include "il.h"
#include <stdlib.h>

Code *new_code(u4 op, ...) {
  va_list(args);

  Code *new_code = (Code *)malloc(sizeof(Code));
  new_code->stub.prev = new_code->stub.next = NULL;
  new_code->lineno = 0;
  new_code->op = op;
  ADDR0(new_code) = ADDR1(new_code) = ADDR2(new_code) = NULL;

  va_start(args, op);
  if (op == OP_FUNC) {
    ADDR0(new_code) = va_arg(args, Address *);
  } else if (op == OP_GLOB) {
    ADDR0(new_code) = va_arg(args, Address *);
    IMM1(new_code) = va_arg(args, u4);
  } else if (op <= OP_GOTO) {
    if (op <= OP_WRITE_V)
      ADDR0(new_code) = va_arg(args, Address *);
    else
      IMM0(new_code) = va_arg(args, u4);
  } else if (op <= OP_MOV_MI) {
    ADDR0(new_code) = va_arg(args, Address *);
    if (op <= OP_MOV_MV)
      ADDR1(new_code) = va_arg(args, Address *);
    else
      IMM1(new_code) = va_arg(args, u4);
  } else {
    ADDR0(new_code) = va_arg(args, Address *);
    if (op <= OP_LESS_VV) {
      ADDR1(new_code) = va_arg(args, Address *);
      ADDR2(new_code) = va_arg(args, Address *);
    } else if (op <= OP_LESS_VI) {
      ADDR1(new_code) = va_arg(args, Address *);
      IMM2(new_code) = va_arg(args, u4);
    } else {
      IMM1(new_code) = va_arg(args, u4);
      ADDR2(new_code) = va_arg(args, Address *);
    }
  }
  va_end(args);

  return new_code;
}

void _show_code(Code *code, FILE *out) {
  const char *code_repr[] = {
      "\nfunc %s:",
      "glob %s %d",

      "(%d) param %s",
      "(%d) arg %s",
      "(%d) ret %s",
      "(%d) read %s",
      "(%d) write %s",
      "(%d) arg %d",
      "(%d) ret %d",
      "(%d) write %d",
      "(%d) goto (%d)",

      "(%d) %s = call %s",
      "(%d) %s = &%s",
      "(%d) %s = %s",
      "(%d) %s = *%s",
      "(%d) *%s = %s",
      "(%d) local %s %d",
      "(%d) %s = %d",
      "(%d) *%s = %d",

      "(%d) %s = %s + %s",
      "(%d) %s = %s - %s",
      "(%d) %s = %s * %s",
      "(%d) %s = %s / %s",
      "(%d) if %s == %s goto (%d)",
      "(%d) if %s != %s goto (%d)",
      "(%d) if %s >= %s goto (%d)",
      "(%d) if %s <= %s goto (%d)",
      "(%d) if %s > %s goto (%d)",
      "(%d) if %s < %s goto (%d)",

      "(%d) %s = %s + %d",
      "(%d) %s = %s - %d",
      "(%d) %s = %s * %d",
      "(%d) %s = %s / %d",
      "(%d) if %s == %d goto (%d)",
      "(%d) if %s != %d goto (%d)",
      "(%d) if %s >= %d goto (%d)",
      "(%d) if %s <= %d goto (%d)",
      "(%d) if %s > %d goto (%d)",
      "(%d) if %s < %d goto (%d)",

      "(%d) %s = %d - %s",
      "(%d) %s = %d / %s",
  };

  const char *repr = code_repr[code->op - OP_FUNC];

  if (code->op == OP_FUNC)
    fprintf(out, repr, ADDR0(code)->name);
  else if (code->op == OP_GLOB)
    fprintf(out, repr, ADDR0(code)->name, IMM1(code));
  else if (code->op <= OP_WRITE_V)
    fprintf(out, repr, ADDR0(code)->name);
  else if (code->op <= OP_GOTO)
    fprintf(out, repr, IMM0(code));
  else if (code->op <= OP_MOV_MV)
    fprintf(out, repr, ADDR0(code)->name, ADDR1(code)->name);
  else if (code->op <= OP_MOV_MI)
    fprintf(out, repr, ADDR0(code)->name, IMM1(code));
  else if (code->op <= OP_LESS_VV)
    fprintf(out, repr, ADDR0(code)->name, ADDR1(code)->name, ADDR2(code)->name);
  else if (code->op <= OP_LESS_VI)
    fprintf(out, repr, ADDR0(code)->name, ADDR1(code)->name, IMM2(code));
  else
    fprintf(out, repr, ADDR0(code)->name, IMM1(code), ADDR2(code)->name);

  fprintf(out, "\n");
  fflush(stdout);
}

void code_list_init(CodeList *cl) {
  hash_map_init(&cl->names, hash_elf, 4096);
  linked_list_init(&cl->codes);
  cl->local_ctr = cl->tmp_ctr = 0;
}

void code_list_free(CodeList *cl) {
  hash_map_free(&cl->names);
  linked_list_free(&cl->codes);
}

void code_list_show(CodeList *cl, FILE *out) {
  LinkedStub *stub = &cl->codes.head;
  while ((stub = stub->next) != NULL)
    _show_code((Code *)stub, out);
}

void code_list_append(CodeList *cl, Code *code) {
  if (linked_list_empty(&cl->codes))
    code->lineno = 1;
  else
    code->lineno = ((Code *)cl->codes.tail)->lineno + 1;

  linked_list_push(&cl->codes, &code->stub);
}

char *_num2name(char *fmt, u4 num) {
  char buf[16];
  sprintf(buf, fmt, num);
  return new_string(buf);
}

Address *gen_glob(CodeList *cl, char *name, u4 mem) {
  Address *glob_var = (Address *)malloc(sizeof(Address));
  glob_var->name = name;
  glob_var->mem = mem;

  hash_map_put(&cl->names, name, glob_var);
  return glob_var;
}

Address *gen_local(CodeList *cl, u4 mem) {
  Address *local_var = (Address *)malloc(sizeof(Address));
  local_var->name = _num2name("v%d", cl->local_ctr++);
  local_var->mem = mem;

  hash_map_put(&cl->names, local_var->name, local_var);
  return local_var;
}

Address *gen_tmp(CodeList *cl) {
  Address *tmp_var = (Address *)malloc(sizeof(Address));
  tmp_var->name = _num2name("t%d", cl->tmp_ctr++);
  tmp_var->mem = 1;

  hash_map_put(&cl->names, tmp_var->name, tmp_var);
  return tmp_var;
}