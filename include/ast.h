#ifndef __AST_H__
#define __AST_H__

#include "mm.h"
#include <stdarg.h>
#include <stdio.h>

typedef struct ASTNode {
  BinaryTreeStub stub;
  enum {
    NON_TOK_PROGRAM,
    NON_TOK_EXTDEFLIST,
    NON_TOK_EXTDEF,
    NON_TOK_DEFLIST,
    NON_TOK_DEF,
    NON_TOK_DECLIST,
    NON_TOK_DEC,
    NON_TOK_FUNDEC,
    NON_TOK_VARLIST,
    NON_TOK_COMPST,
    NON_TOK_STMTLIST,
    NON_TOK_STMT,
    NON_TOK_EXP,

    NON_TOK_ARGS,
    TOK_INT,
    TOK_SEMI,
    TOK_COMMA,
    TOK_RELOP,
    TOK_ASSIGNOP,
    TOK_PLUS,
    TOK_MINUS,
    TOK_STAR,
    TOK_DIV,
    TOK_AND,
    TOK_OR,
    TOK_NOT,
    TOK_LP,
    TOK_RP,
    TOK_LB,
    TOK_RB,
    TOK_LC,
    TOK_RC,
    TOK_RETURN,
    TOK_IF,
    TOK_ELSE,
    TOK_WHILE,
    TOK_READ,
    TOK_WRITE,
    TOK_TYPE,
    TOK_ID,
  } type;
  union {
    struct {
      char *text;
    } token;
    struct {
      u4 ruleno;
      u4 lineno;
    } non_token;
  } attr;
} ASTNode;

#define TOKEN(ASTN) ((ASTN)->attr.token)
#define NON_TOKEN(ASTN) ((ASTN)->attr.non_token)
#define AST_ROOT(AST) ((ASTNode *)(AST)->hierachy.root)

void _show_indent(FILE *out, u4 indent);
void _show_token(ASTNode *node, FILE *out);
void _show_non_token(ASTNode *node, FILE *out);
void _show_ast_node(ASTNode *node, FILE *out, u4 indent);

typedef struct ASTree {
  BinaryTree hierachy;
  ArrayList tokens;
  ArrayList non_tokens;
} ASTree;

void ast_init(ASTree *ast);
void ast_free(ASTree *ast);
void ast_reduct(ASTNode *rule, u4 ctr, ...);
#define ast_show(AST, OUT) _show_ast_node(AST_ROOT(AST), OUT, 0)

ASTNode *new_token(ASTree *ast, u4 type, char *text);
ASTNode *new_non_token(ASTree *ast, u4 type, u4 ruleno, u4 lineno);
ASTNode *get_node(ASTNode *par, u4 prodno);

#endif