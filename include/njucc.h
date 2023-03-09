#ifndef __NJUCC_H__
#define __NJUCC_H__

#include "sdt.h"
#include <stdio.h>

typedef struct Runtime {
  FILE *src;
  FILE *ast_out;
  FILE *il_out;
  FILE *asm_out;
  ASTree ast;
  SymbolTable symbols;
  CodeList il;
  u1 is_dbg;
  u1 err_flg;
} Runtime;

#define ERROR(LABEL) void LABEL##_error(const char *msg, u4 where)

ERROR(parse);
ERROR(syntax);
ERROR(semantic);

#define PASSED 0b0
#define PARSE_ERR 0b1
#define SYNTAX_ERR 0b10
#define SEMANTIC_ERR 0b100

#endif