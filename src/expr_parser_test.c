/** Interpretr jazyka IFJ16
* @file expr_parser_test.c
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#include "gc.h"
#include "scanner.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
   gc_init();
   
   FILE *file = fopen(argv[1], "r");

   Tinit *scanner = init_scanner(file);

   printf("%x\n",parseExpression(scanner));
   return 1;   
}