/** Interpretr jazyka IFJ16
* @file main.c
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/
#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "gc.h"


int main(int argc, char *argv[])
{
   gc_init();

   if (argc == 1)
      return -1;
   FILE *fp;

   fp = fopen(argv[1], "r");

   if (fp == NULL)
   {
      fprintf(stderr, "Error opening file!\n");
      return 99;
   }

   parse_program(init_syntax(fp));
   fclose(fp);
   return 0;
}
