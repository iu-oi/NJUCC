#ifndef __IL_H__
#define __IL_H__

#include "mm.h"
#include <stdarg.h>
#include <stdio.h>

typedef struct Variable {
  char *name;
  u4 mem;
} Variable;

typedef struct Code {
  LinkedStub stub;
  u4 lineno;
  enum {
    OP_FUNC,
    OP_PARAM,
    OP_ARG,
    OP_RET,
    OP_READ,
    OP_WRITE,

    OP_ARGI,
    OP_RETI,
    OP_WRITEI,
    OP_GOTO,

    OP_CALL,
    OP_ASSIGN,
    OP_ASSIGNA,
    OP_ASSIGNM,
    OP_MASSIGN,

    OP_GLOB,
    OP_LOCAL,
    OP_ASSIGNI,

    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,

    OP_ADDI,
    OP_SUBI,
    OP_MULI,
    OP_DIVI,

    OP_ISUB,
    OP_IDIV,

    OP_EQ,
    OP_NEQ,
    OP_GE,
    OP_LE,
    OP_GTR,
    OP_LESS,

    OP_EQI,
    OP_NEQI,
    OP_GEI,
    OP_LEI,
    OP_GTRI,
    OP_LESSI
  } op;
  Variable *dest;
  union {
    u8 wild;
    Variable *var;
    u4 imm;
  } src1, src2;
} Code;

Code *new_code(u4 op, ...);
void _show_code(Code *code, FILE *out);

typedef struct CodeList {
  HashMap vars;
  LinkedList list;
  u4 local_ctr;
  u4 tmp_ctr;
} CodeList;

void code_list_init(CodeList *cl);
void code_list_free(CodeList *cl);
void code_list_show(CodeList *cl, FILE *out);
void code_list_append(CodeList *cl, Code *code);

#define gen_code(CODEL, CODET, ...)                                            \
  code_list_append(CODEL, new_code(CODET, __VA_ARGS__))

char *_num2name(char *fmt, u4 num);

Variable *gen_glob(CodeList *cl, char *name, u4 mem);
Variable *gen_local(CodeList *cl, u4 mem);
Variable *gen_tmp(CodeList *cl);

#endif