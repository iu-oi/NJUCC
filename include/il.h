#ifndef __IL_H__
#define __IL_H__

#include "mm.h"
#include <stdarg.h>
#include <stdio.h>

typedef struct Address {
  char *name;
  u4 mem;
} Address;

typedef struct Code {
  LinkedStub stub;
  u4 lineno;
  enum {
    OP_FUNC,
    OP_GLOB,

    OP_PARAM,
    OP_ARG_V,
    OP_RET_V,
    OP_READ_V,
    OP_WRITE_V,
    OP_ARG_I,
    OP_RET_I,
    OP_READ_I,
    OP_WRITE_I,
    OP_GOTO,

    OP_CALL,
    OP_MOV_VA,
    OP_MOV_VV,
    OP_MOV_VM,
    OP_MOV_MV,
    OP_LOCAL,
    OP_MOV_VI,
    OP_MOV_MI,

    OP_ADD_VV,
    OP_SUB_VV,
    OP_MUL_VV,
    OP_DIV_VV,
    OP_EQ_VV,
    OP_NEQ_VV,
    OP_GE_VV,
    OP_LE_VV,
    OP_GTR_VV,
    OP_LESS_VV,
    OP_ADD_VI,
    OP_SUB_VI,
    OP_MUL_VI,
    OP_DIV_VI,
    OP_EQ_VI,
    OP_NEQ_VI,
    OP_GE_VI,
    OP_LE_VI,
    OP_GTR_VI,
    OP_LESS_VI,
    OP_SUB_IV,
    OP_DIV_IV
  } op;
  union {
    Address *addr;
    u4 imm;
  } arg[3];
} Code;

#define ADDR0(CODE) ((CODE)->arg[0].addr)
#define ADDR1(CODE) ((CODE)->arg[1].addr)
#define ADDR2(CODE) ((CODE)->arg[2].addr)
#define IMM0(CODE) ((CODE)->arg[0].imm)
#define IMM1(CODE) ((CODE)->arg[1].imm)
#define IMM2(CODE) ((CODE)->arg[2].imm)

Code *new_code(u4 op, ...);
void _show_code(Code *code, FILE *out);

typedef struct CodeList {
  HashMap names;
  LinkedList codes;
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

Address *gen_glob(CodeList *cl, char *name, u4 mem);
Address *gen_local(CodeList *cl, u4 mem);
Address *gen_tmp(CodeList *cl);

#endif