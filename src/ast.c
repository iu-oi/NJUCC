#include "ast.h"
#include <stdlib.h>
#include <string.h>

void _show_indent(FILE *out, u4 indent) {
  while (indent--)
    fprintf(out, " ");
}

void _show_token(ASTNode *node, FILE *out) {
  const char *TOKEN_REPR[] = {
      "INT: %d", "SEMI", "COMMA", "RELOP: %s", "ASSIGNOP", "PLUS", "MINUS",
      "STAR",    "DIV",  "AND",   "OR",        "NOT",      "LP",   "RP",
      "LB",      "RB",   "LC",    "RC",        "RETURN",   "IF",   "ELSE",
      "WHILE",   "READ", "WRITE", "TYPE: %s",  "ID: %s"};

  if (node->type == TOK_INT)
    fprintf(out, TOKEN_REPR[0], strtol(TOKEN(node).text, NULL, 0));
  else
    fprintf(out, TOKEN_REPR[node->type - TOK_INT], TOKEN(node).text);
  fprintf(out, "\n");
  fflush(out);
}

void _show_non_token(ASTNode *node, FILE *out) {
  const char *NON_TOKEN_REPR[] = {
      "Program (%d)", "ExtDefList (%d)", "ExtDef (%d)",   "DefList (%d)",
      "Def (%d)",     "DecList (%d)",    "Dec (%d)",      "FunDec (%d)",
      "VarList (%d)", "CompSt (%d)",     "StmtList (%d)", "Stmt (%d)",
      "Exp (%d)",     "Args (%d)"};

  fprintf(out, NON_TOKEN_REPR[node->type - NON_TOK_PROGRAM],
          NON_TOKEN(node).lineno);
  fprintf(out, "\n");
  fflush(out);
}

void _show_ast_node(ASTNode *node, FILE *out, u4 indent) {
  if (node->type >= TOK_INT) {
    _show_indent(out, indent);
    _show_token(node, out);
  } else if (NON_TOKEN(node).ruleno != 0) {
    _show_indent(out, indent);
    _show_non_token(node, out);
    BinaryTreeStub *stub = node->stub.left;
    while (stub != NULL) {
      _show_ast_node((ASTNode *)stub, out, indent + 1);
      stub = stub->right;
    }
  }
}

void ast_init(ASTree *ast) {
  binary_tree_init(&ast->hierachy);
  array_list_init(&ast->tokens, LONG_ARR_LST);
  array_list_init(&ast->non_tokens, LONG_ARR_LST);
}

void ast_free(ASTree *ast) {
  array_list_free(&ast->tokens);
  array_list_free(&ast->non_tokens);
}

void ast_reduct(ASTNode *rule, u4 ctr, ...) {
  va_list prods;

  va_start(prods, ctr);
  while (ctr--) {
    BinaryTreeStub *prod = va_arg(prods, BinaryTreeStub *);
    prod->right = rule->stub.left;
    rule->stub.left = prod;
  }
  va_end(prods);
}

ASTNode *new_token(ASTree *ast, u4 type, char *text) {
  ASTNode *new_node = (ASTNode *)malloc(sizeof(ASTNode));
  new_node->type = type;
  new_node->stub.left = new_node->stub.right = NULL;

  TOKEN(new_node).text = new_string(text);
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

ASTNode *get_node(ASTNode *par, u4 prodno) {
  BinaryTreeStub *stub = par->stub.left;
  do {
    if (--prodno == 0)
      return (ASTNode *)stub;
  } while ((stub = stub->right) != NULL);

  return NULL;
}