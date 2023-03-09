#ifndef __CODE_GEN_H__
#define __CODE_GEN_H__

#include "data.h"
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

Code *new_code(u4, ...);
void _show_code(Code *, FILE *);

typedef struct CodeSegment {
  HashMap vars;
  LinkedList codel;
  u4 local_ctr;
  u4 tmp_ctr;
} CodeSegment;

void code_seg_init(CodeSegment *);
void code_seg_free(CodeSegment *);
void code_seg_show(CodeSegment *, FILE *);
void code_seg_append(CodeSegment *, Code *);

#define gen_code(CODE_SEG, CODE_TYP, ...)                                      \
  code_seg_append(CODE_SEG, new_code(CODE_TYP, __VA_ARGS__))

char *_num2name(char *, u4);

Variable *gen_glob(CodeSegment *, char *, u4);
Variable *gen_local(CodeSegment *, u4);
Variable *gen_tmp(CodeSegment *);

#endif