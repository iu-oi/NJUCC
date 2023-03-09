#ifndef __ASTREE_PARSE_H__
#define __ASTREE_PARSE_H__

#include "ASTree.h"
#include "code_gen.h"
#include "symb_table.h"

#define SDT_PARAM ASTNode *node, SymbolTable *table
#define SDT(LABEL) sdt_##LABEL

void SDT(program)(SDT_PARAM);
void SDT(ext_def_list)(SDT_PARAM);
void SDT(ext_def)(SDT_PARAM);
void SDT(def_list)(SDT_PARAM);
void SDT(def)(SDT_PARAM);
void SDT(dec_list)(SDT_PARAM);
SymbolField *SDT(dec)(SDT_PARAM, SymbolType *);
void SDT(fun_dec)(SDT_PARAM);
void SDT(var_list)(SDT_PARAM, SymbolFunc *);
void SDT(comp_st)(SDT_PARAM);
void SDT(stmt_list)(SDT_PARAM);
void SDT(stmt)(SDT_PARAM);
SymbolType *SDT(exp)(SDT_PARAM);
void SDT(args)(SDT_PARAM, SymbolField *);

#endif