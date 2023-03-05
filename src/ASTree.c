#include "ASTree.h"
#include <stdlib.h>
#include <string.h>

void ast_init(ASTree *ast) {
  binary_tree_init(&ast->hierachy);
  array_list_init(&ast->tokens, 128);
  array_list_init(&ast->non_tokens, 128);
  hash_map_init(&ast->texts, hash_elf, 4096);
}

void ast_show(ASTree *ast, FILE *out) {
  _show_ast_node((ASTNode *)ast->hierachy.root, out, 0);
}

void ast_reduct(ASTNode *rule, u4 ctr, ...) {
  va_list prods;

  va_start(prods, ctr);

  while (ctr--) {
    BinaryTreeStub *prod = va_arg(prods, BinaryTreeStub *);
    binary_tree_addr(prod, rule->stub.left);
    binary_tree_addl(&rule->stub, prod);
  }

  va_end(prods);
}

ASTNode *get_node(ASTNode *par, u4 prodno) {
  BinaryTreeStub *stub = par->stub.left;
  do {
    if (--prodno == 0)
      return (ASTNode *)stub;
  } while ((stub = stub->right) != NULL);
  return NULL;
}

void ast_free(ASTree *ast) {
  array_list_free(&ast->tokens);
  array_list_free(&ast->non_tokens);
  hash_map_free(&ast->texts);
}

ASTNode *new_token(ASTree *ast, u4 type, char *text) {
  ASTNode *new_node = (ASTNode *)malloc(sizeof(ASTNode));
  new_node->type = type;
  new_node->stub.left = new_node->stub.right = NULL;

  char *text_copy = (char *)hash_map_get(&ast->texts, text);
  if (text_copy == NULL) {
    text_copy = (char *)malloc(strlen(text) + 1);
    strcpy(text_copy, text);
    hash_map_put(&ast->texts, text, text_copy);
  }

  TOKEN(new_node).text = text_copy;
  array_list_push(&ast->tokens, new_node);

  return new_node;
}

ASTNode *new_non_token(ASTree *ast, u4 type, u4 ruleno, u4 lineno) {
  ASTNode *new_node = (ASTNode *)malloc(sizeof(ASTNode));
  new_node->type = type;
  new_node->stub.left = new_node->stub.right = NULL;

  NON_TOKEN(new_node).ruleno = ruleno;
  NON_TOKEN(new_node).lineno = lineno;
  array_list_push(&ast->non_tokens, new_node);

  return new_node;
}

void _show_token(ASTNode *node, FILE *out, u4 indent) {
  const char *token_repr[] = {
      "INT: %d", "SEMI", "COMMA", "RELOP: %s", "ASSIGNOP", "PLUS", "MINUS",
      "STAR",    "DIV",  "AND",   "OR",        "NOT",      "LP",   "RP",
      "LB",      "RB",   "LC",    "RC",        "RETURN",   "IF",   "ELSE",
      "WHILE",   "READ", "WRITE", "TYPE: %s",  "ID: %s"};

  while (indent--)
    fprintf(out, " ");

  if (node->type == TOK_INT)
    fprintf(out, token_repr[0], strtol(TOKEN(node).text, NULL, 0));
  else
    fprintf(out, token_repr[node->type - TOK_INT], TOKEN(node).text);

  fprintf(out, "\n");
  fflush(out);
}

void _show_non_token(ASTNode *node, FILE *out, u4 indent) {
  const char *non_token_repr[] = {
      "Program (%d)", "ExtDefList (%d)", "ExtDef (%d)",   "DefList (%d)",
      "Def (%d)",     "DecList (%d)",    "Dec (%d)",      "FunDec (%d)",
      "VarList (%d)", "CompSt (%d)",     "StmtList (%d)", "Stmt (%d)",
      "Exp (%d)",     "Args (%d)"};

  if (NON_TOKEN(node).ruleno != 0) {
    while (indent--)
      fprintf(out, " ");

    fprintf(out, non_token_repr[node->type - NON_TOK_PROGRAM],
            NON_TOKEN(node).lineno);

    fprintf(out, "\n");
    fflush(out);
  }
}

void _show_ast_node(ASTNode *node, FILE *out, u4 indent) {
  if (node->type >= TOK_INT) {
    _show_token(node, out, indent);
  } else {
    _show_non_token(node, out, indent);
    BinaryTreeStub *stub = node->stub.left;
    while (stub != NULL) {
      _show_ast_node((ASTNode *)stub, out, indent + 1);
      stub = stub->right;
    }
  }
}