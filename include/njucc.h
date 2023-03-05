#ifndef __NJUCC_H__
#define __NJUCC_H__

#include "data.h"
#include "ASTree.h"
#include "ASTree_parse.h"
#include <stdio.h>

typedef struct Runtime {
  FILE *src;
  FILE *ast_out;
  FILE *ic_out;
  FILE *asm_out;
  ASTree ast;
  SymbolTable symbols;
  u1 is_dbg;
} Runtime;

#endif