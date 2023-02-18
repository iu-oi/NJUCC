#include "njucc.h"
#include <string.h>

int yyrestart(FILE *);
int yyparse();

Runtime rt;

int main(int argc, char *argv[], char **envp) {
  const char help_msg[] =
      "Usage: njucc [file]... [options]...\n"
      "\t-a <file>  : Write abstract syntax tree into <file>.\n"
      "\t-i <file>  : Write intermediate code into <file>.\n"
      "\t-s <file>  : Write target assembly into <file>.\n"
      "\t-d --debug : Show compiler debugging info.\n";

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
        arg_out = &rt.ic_out;
      else if (!strcmp(argv[argi], "-s"))
        arg_out = &rt.asm_out;
      else
        goto bad_usage;
    } else
      goto bad_usage;
  }

  /* generate abstract syntax tree */
  ast_init(&rt.ast);
  yyrestart(rt.src);
  yyparse();
  if (rt.is_dbg)
    ast_show(&rt.ast, stdout);
  if (rt.ast_out)
    ast_show(&rt.ast, rt.ast_out);

  /* todo semantic check here */
  /* todo inter code generation here */

  ast_free(&rt.ast);
  return 0;

bad_usage:
  printf("%s", help_msg);
  return 1;

bad_file:
  perror(argv[argi]);
  return 1;
}