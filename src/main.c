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
	void * q = NULL;
	for (int i=0; i!=50; i++) {
		q = gc_alloc(50);
		gc_free(q);
	}
	return 0;	
}
