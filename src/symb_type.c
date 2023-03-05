#include "symb_type.h"
#include <stdlib.h>

SymbolType *new_symbol_type(SymbolType *subtype, u4 len) {
  BinaryTreeStub *stub = &subtype->stub;

  stub = stub->left;
  while (stub != NULL) {
    if (((SymbolType *)stub)->len == len)
      return (SymbolType *)stub;
    stub = stub->right;
  }

  SymbolType *new_type = (SymbolType *)malloc(sizeof(SymbolType));
  new_type->stub.left = new_type->stub.right = NULL;
  new_type->subtype = subtype;
  new_type->len = len;
  new_type->mem = subtype->mem * len;

  binary_tree_addr(&new_type->stub, subtype->stub.left);
  binary_tree_addl(&subtype->stub, &new_type->stub);

  return new_type;
}

void type_tree_init(TypeTree *tt) {
  SymbolType *basic_type = (SymbolType *)malloc(sizeof(SymbolType));
  basic_type->stub.left = basic_type->stub.right = NULL;
  basic_type->subtype = NULL;
  basic_type->len = basic_type->mem = 1;

  binary_tree_init(tt);
  tt->root = &basic_type->stub;
}