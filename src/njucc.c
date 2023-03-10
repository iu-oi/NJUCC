#include "njucc.h"
#include <string.h>

int yyrestart(FILE *);
int yyparse();

Runtime rt;

int main(int argc, char *argv[], char **envp) {
  const char HELP_MSG[] =
      "Usage: njucc [file]... [options]...\n"
      "\t-a <file>  : Write abstract syntax tree into <file>\n"
      "\t-i <file>  : Write 3 addr code into <file>\n"
      "\t-s <file>  : Write assembly into <file>\n"
      "\t-d --debug : Show compiler debugging info\n";

  if (argc < 2)
    goto bad_usage;

  u4 argi = 0;
  FILE **arg_in = &rt.src;
  FILE **arg_out = NULL;

  while (++argi < argc) {
    if (arg_in) {
      if (!(*arg_in = fopen(argv[argi], "r")))
        goto bad_file;
      arg_in = NULL;
    } else if (arg_out) {
      if (!(*arg_out = fopen(argv[argi], "w")))
        goto bad_file;
      arg_out = NULL;
    } else if (argv[argi][0] == '-') {
      if (!strcmp(argv[argi], "-d") || !strcmp(argv[argi], "--debug"))
        rt.is_dbg = 1;
      else if (!strcmp(argv[argi], "-a"))
        arg_out = &rt.ast_out;
      else if (!strcmp(argv[argi], "-i"))
        arg_out = &rt.il_out;
      else if (!strcmp(argv[argi], "-s"))
        arg_out = &rt.asm_out;
      else
        goto bad_usage;
    } else
      goto bad_usage;
  }

  rt.err_flg = PASSED;
  string_set_init();

  /* generate abstract syntax tree */
  ast_init(&rt.ast);
  yyrestart(rt.src);
  yyparse();

  if (rt.err_flg == PASSED) {
    /* show abstract syntax tree */
    if (rt.is_dbg)
      ast_show(&rt.ast, stdout);
    if (rt.ast_out)
      ast_show(&rt.ast, rt.ast_out);

    symbol_table_init(&rt.symbols);
    code_list_init(&rt.il);

    /* parse abstract syntax tree */
    sdt_program(AST_ROOT(&rt.ast), &rt.symbols);

    if (rt.err_flg == PASSED) {
      /* show intermediate representation */
      if (rt.is_dbg)
        code_list_show(&rt.il, stdout);
      if (rt.il_out)
        code_list_show(&rt.il, rt.il_out);
    }

    code_list_free(&rt.il);
    symbol_table_free(&rt.symbols);
  }

  ast_free(&rt.ast);

  string_set_free();
  return 0;

bad_usage:
  printf("%s", HELP_MSG);
  return 1;

bad_file:
  perror(argv[argi]);
  return 1;
}

ERROR(parse) {
  rt.err_flg |= PARSE_ERR;
  printf("Parse error at line %d: %s\n", where, msg);
  fflush(stdout);
}

u4 last_syntax_err = 0;

ERROR(syntax) {
  if (where > last_syntax_err)
    last_syntax_err = where;
  else
    return;

  if ((rt.err_flg & ~SYNTAX_ERR) == PASSED) {
    rt.err_flg |= SYNTAX_ERR;
    printf("Syntax error at line %d: %s\n", where, msg);
    fflush(stdout);
  }
}

u4 last_semantic_err = 0;

ERROR(semantic) {
  if (where > last_semantic_err)
    last_semantic_err = where;
  else
    return;

  if ((rt.err_flg & ~SEMANTIC_ERR) == PASSED) {
    rt.err_flg |= SEMANTIC_ERR;
    printf("Semantic error at line %d: %s\n", where, msg);
    fflush(stdout);
  }
}