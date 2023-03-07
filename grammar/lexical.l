%option yylineno
%option nounput

%{
  #include "njucc.h"
  #include "ASTree.h"
  #include "syntax.tab.h"

  #define YY_USER_ACTION \
    yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; \
    yylloc.last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng;

  int yycolumn = 1;

  extern Runtime rt;

  #define gen_token(TOK, TYPE) \
    yylval.node = new_token(&rt.ast, TYPE, yytext); \
    return TOK;
%}

oct 0[0-7]+
oct_err 0[0-9]+
hex 0[xX][0-9A-Fa-f]+
hex_err 0[xX][0-9A-Za-z]+
int [1-9][0-9]*|0|{oct}|{hex}
int_err {oct_err}|{hex_err}
id [_a-zA-Z][_0-9a-zA-Z]*
sep [ \f\r\t\v]

%%

"//" { while (input() != '\n'); }

"/*" {
  char c;
  int s = 0;
  while ((c = input()) != 0)
  {
    if (c == '/' && s) break;
    if (c == '*') s = 1;
    else s = 0;
  }
}

{int} { gen_token(INT, TOK_INT) }

{int_err} {
  parse_error("bad integer", yylineno);
  gen_token(INT, TOK_INT)
}

";" { gen_token(SEMI, TOK_SEMI) }

"," { gen_token(COMMA, TOK_COMMA) }

"==" { gen_token(RELOP, TOK_RELOP) }

"!=" { gen_token(RELOP, TOK_RELOP) }

">=" { gen_token(RELOP, TOK_RELOP) }

"<=" { gen_token(RELOP, TOK_RELOP) }

">" { gen_token(RELOP, TOK_RELOP) }

"<" { gen_token(RELOP, TOK_RELOP) }

"=" { gen_token(ASSIGNOP, TOK_ASSIGNOP) }

"+" { gen_token(PLUS, TOK_PLUS) }

"-" { gen_token(MINUS, TOK_MINUS) }

"*" { gen_token(STAR, TOK_STAR) }

"/" { gen_token(DIV, TOK_DIV) }

"&&" { gen_token(AND, TOK_AND) }

"||" { gen_token(OR, TOK_OR) }

"!" { gen_token(NOT, TOK_NOT) }

"(" { gen_token(LP, TOK_LP) }

")" { gen_token(RP, TOK_RP) }

"[" { gen_token(LB, TOK_LB) }

"]" { gen_token(RB, TOK_RB) }

"{" { gen_token(LC, TOK_LC) }

"}" { gen_token(RC, TOK_RC) }

"return" { gen_token(RETURN, TOK_RETURN) }

"if" { gen_token(IF, TOK_IF) }

"else" { gen_token(ELSE, TOK_ELSE) }

"while" { gen_token(WHILE, TOK_WHILE) }

"read" { gen_token(READ, TOK_READ) }

"write" { gen_token(WRITE, TOK_WRITE) }

"int" { gen_token(TYPE, TOK_TYPE) }

{id} { gen_token(ID, TOK_ID) }

{sep} { }

\n { yycolumn = 1; }

. { parse_error("unknown character(s)", yylineno); }

%%
