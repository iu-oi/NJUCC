%locations
%define parse.error verbose

%{
  #include "lex.yy.c"

  #define gen_non_token(NON_TOK, TYPE, RULENO, LINENO) \
  	NON_TOK = new_non_token(&rt.ast, TYPE, RULENO, LINENO);

  int yyerror(const char *);
%}

%union { ASTNode *node; }

%token <node> INT"digit(s)"
%token <node> SEMI";" COMMA","
%token <node> RELOP"op" ASSIGNOP"="
%token <node> PLUS"+" MINUS"-" STAR"*" DIV"/"
%token <node> AND"&&" OR"||" NOT"!"
%token <node> TYPE"type" ID"id"
%token <node> LP"(" RP")" LB"[" RB"]" LC"{" RC"}"
%token <node> RETURN"return" IF"if" ELSE"else" WHILE"while"
%token <node> READ"read" WRITE"write"

%type <node> Program
%type <node> ExtDefList ExtDef
%type <node> DefList Def 
%type <node> DecList Dec
%type <node> FunDec VarList
%type <node> CompSt
%type <node> StmtList Stmt
%type <node> Exp Args

%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left LP RP LB RB

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

Program : ExtDefList {
	gen_non_token($$, NON_TOK_PROGRAM, 1, @$.first_line)
	ast_reduct($$, 1, $1);
	rt.ast.hierachy.root = (BinaryTreeStub *)$$;
};

ExtDefList : ExtDef ExtDefList {
	gen_non_token($$, NON_TOK_EXTDEFLIST, 1, @$.first_line)
	ast_reduct($$, 2, $2, $1);
} | {
	gen_non_token($$, NON_TOK_EXTDEFLIST, 0, @$.first_line)
};
	
ExtDef : TYPE DecList SEMI {
	gen_non_token($$, NON_TOK_EXTDEF, 1, @$.first_line)
	ast_reduct($$, 3, $3, $2, $1);
} | TYPE FunDec CompSt {
	gen_non_token($$, NON_TOK_EXTDEF, 2, @$.first_line)
	ast_reduct($$, 3, $3, $2, $1);
} | error SEMI {}; 

DefList : Def DefList {
	gen_non_token($$, NON_TOK_DEFLIST, 1, @$.first_line)
	ast_reduct($$, 2, $2, $1);
} | {
	gen_non_token($$, NON_TOK_DEFLIST, 0, @$.first_line)
};

Def : TYPE DecList SEMI {
	gen_non_token($$, NON_TOK_DEF, 1, @$.first_line)
	ast_reduct($$, 3, $3, $2, $1);
} | TYPE error SEMI {};

DecList : Dec {
	gen_non_token($$, NON_TOK_DECLIST, 1, @$.first_line)
	ast_reduct($$, 1, $1);
} | Dec COMMA DecList {
	gen_non_token($$, NON_TOK_DECLIST, 2, @$.first_line)	
	ast_reduct($$, 3, $3, $2, $1);
};

Dec : ID {
	gen_non_token($$, NON_TOK_DEC, 1, @$.first_line)
	ast_reduct($$, 1, $1);
} | Dec LB INT RB {
	gen_non_token($$, NON_TOK_DEC, 2, @$.first_line)
	ast_reduct($$, 4, $4, $3, $2, $1);
};

FunDec : ID LP RP {
	gen_non_token($$, NON_TOK_FUNDEC, 1, @$.first_line)	
	ast_reduct($$, 3, $3, $2, $1);
} | ID LP VarList RP {
	gen_non_token($$, NON_TOK_FUNDEC, 2, @$.first_line)	
	ast_reduct($$, 4, $4, $3, $2, $1);
} | error RP {};

VarList : TYPE Dec {
	gen_non_token($$, NON_TOK_VARLIST, 1, @$.first_line)
	ast_reduct($$, 2, $2, $1);
} | TYPE Dec COMMA VarList {
	gen_non_token($$, NON_TOK_VARLIST, 2, @$.first_line)
	ast_reduct($$, 4, $4, $3, $2, $1);
};

CompSt : LC DefList StmtList RC {
	gen_non_token($$, NON_TOK_COMPST, 1, @$.first_line)
	ast_reduct($$, 4, $4, $3, $2, $1);
} | error RC {};

StmtList : Stmt StmtList {
	gen_non_token($$, NON_TOK_STMTLIST, 1, @$.first_line)
	ast_reduct($$, 2, $2, $1);
} | {
	gen_non_token($$, NON_TOK_STMTLIST, 0, @$.first_line)
};

Stmt : Exp SEMI {
	gen_non_token($$, NON_TOK_STMT, 1, @$.first_line)
	ast_reduct($$, 2, $2, $1);
} | CompSt {
	gen_non_token($$, NON_TOK_STMT, 2, @$.first_line)
	ast_reduct($$, 1, $1);
} | RETURN Exp SEMI {
	gen_non_token($$, NON_TOK_STMT, 3, @$.first_line)
	ast_reduct($$, 3, $3, $2, $1);
} | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {
	gen_non_token($$, NON_TOK_STMT, 4, @$.first_line)
	ast_reduct($$, 5, $5, $4, $3, $2, $1);
} | IF LP Exp RP Stmt ELSE Stmt {
	gen_non_token($$, NON_TOK_STMT, 5, @$.first_line)
	ast_reduct($$, 7, $7, $6, $5, $4, $3, $2, $1);
} | WHILE LP Exp RP Stmt {
	gen_non_token($$, NON_TOK_STMT, 6, @$.first_line)
	ast_reduct($$, 5, $5, $4, $3, $2, $1);
} | error SEMI {};

Exp : LP Exp RP {
	gen_non_token($$, NON_TOK_EXP, 1, @$.first_line)
	ast_reduct($$, 3, $3, $2, $1);
} | Exp ASSIGNOP Exp {
	gen_non_token($$, NON_TOK_EXP, 2, @$.first_line)
	ast_reduct($$, 3, $3, $2, $1);
} | Exp PLUS Exp {
	gen_non_token($$, NON_TOK_EXP, 3, @$.first_line)
	ast_reduct($$, 3, $3, $2, $1);
} | Exp MINUS Exp {
	gen_non_token($$, NON_TOK_EXP, 4, @$.first_line)
	ast_reduct($$, 3, $3, $2, $1);
} | Exp STAR Exp {
	gen_non_token($$, NON_TOK_EXP, 5, @$.first_line)
	ast_reduct($$, 3, $3, $2, $1);
} | Exp DIV Exp {
	gen_non_token($$, NON_TOK_EXP, 6, @$.first_line)
	ast_reduct($$, 3, $3, $2, $1);
} | MINUS Exp {
	gen_non_token($$, NON_TOK_EXP, 7, @$.first_line)
	ast_reduct($$, 2, $2, $1);
} | NOT Exp {
	gen_non_token($$, NON_TOK_EXP, 8, @$.first_line)
	ast_reduct($$, 2, $2, $1);
} | Exp AND Exp {
	gen_non_token($$, NON_TOK_EXP, 9, @$.first_line)
	ast_reduct($$, 3, $3, $2, $1);
} | Exp OR Exp {
	gen_non_token($$, NON_TOK_EXP, 10, @$.first_line)
	ast_reduct($$, 3, $3, $2, $1);
} | Exp RELOP Exp {
	gen_non_token($$, NON_TOK_EXP, 11, @$.first_line)
	ast_reduct($$, 3, $3, $2, $1);
} | READ LP RP {
	gen_non_token($$, NON_TOK_EXP, 12, @$.first_line)
	ast_reduct($$, 3, $3, $2, $1);
} | WRITE LP Exp RP {
	gen_non_token($$, NON_TOK_EXP, 13, @$.first_line)
	ast_reduct($$, 4, $4, $3, $2, $1);
} | ID LP RP {
	gen_non_token($$, NON_TOK_EXP, 14, @$.first_line)
	ast_reduct($$, 2, $2, $1);
} | ID LP Args RP {
	gen_non_token($$, NON_TOK_EXP, 15, @$.first_line)
	ast_reduct($$, 4, $4, $3, $2, $1);
} | Exp LB Exp RB {
	gen_non_token($$, NON_TOK_EXP, 16, @$.first_line)
	ast_reduct($$, 4, $4, $3, $2, $1);
} | ID {
	gen_non_token($$, NON_TOK_EXP, 17, @$.first_line)
	ast_reduct($$, 1, $1);
} | INT {
	gen_non_token($$, NON_TOK_EXP, 18, @$.first_line)
	ast_reduct($$, 1, $1);
};
	
Args : Exp {
	gen_non_token($$, NON_TOK_ARGS, 1, @$.first_line)
	ast_reduct($$, 1, $1);
} | Exp COMMA Args {
	gen_non_token($$, NON_TOK_ARGS, 2, @$.first_line)
	ast_reduct($$, 3, $3, $2, $1);
};

%%

int yyerror(const char *msg) {
	const char *msg_body = strstr(msg, ",") + 2;
	syntax_error(msg_body, yylineno);
	return 0;
}