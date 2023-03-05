#ifndef __ASTREE_H__
#define __ASTREE_H__

#include "data.h"
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

typedef struct ASTree {
  BinaryTree hierachy;
  ArrayList tokens;
  ArrayList non_tokens;
  HashMap texts;
} ASTree;

void ast_init(ASTree *);
void ast_show(ASTree *, FILE *);
void ast_reduct(ASTNode *, u4, ...);
ASTNode *get_node(ASTNode *, u4);
void ast_free(ASTree *);

ASTNode *new_token(ASTree *, u4, char *);
ASTNode *new_non_token(ASTree *, u4, u4, u4);

void _show_token(ASTNode *, FILE *, u4);
void _show_non_token(ASTNode *, FILE *, u4);
void _show_ast_node(ASTNode *, FILE *, u4);

#endif