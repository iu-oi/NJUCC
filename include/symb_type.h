#ifndef __SYMB_TYPE_H__
#define __SYMB_TYPE_H__

#include "data.h"

typedef struct SymbolType {
  BinaryTreeStub stub;
  struct SymbolType *subtype;
  u4 len;
  u4 mem;
} SymbolType;

SymbolType *new_symbol_type(SymbolType *, u4);

typedef BinaryTree TypeTree;

void type_tree_init(TypeTree *);

#define type_tree_free binary_tree_free

#endif