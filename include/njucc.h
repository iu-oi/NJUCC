#ifndef __NJUCC_H__
#define __NJUCC_H__

#include "ASTree.h"
#include "data.h"
#include <stdio.h>

typedef struct Runtime {
  FILE *src;
  FILE *ast_out;
  FILE *ic_out;
  FILE *asm_out;
  ASTree ast;
  u1 is_dbg;
} Runtime;

#endif