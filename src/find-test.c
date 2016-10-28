/** Interpretr jazyka IFJ16
* @file find-test.c
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#include "ial.h"
#include <stdio.h>

int main()
{
	char* search = "abc";
    char* string = "1123456789abcdefghijklmnabcd";
	printf("pozice: %d \n", find(string, search));
	return 0;
}
