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
    char* string = "123456789abcdefghijklmnabcd";
    
    printf("hledam \"%s\" v retezci %s\n", search, string);
    
	printf("pozice: %d \n", find(string, search));
	
//	------	//
	char* search2 = "";
    
    printf("hledam \"%s\" v retezci %s\n", search2, string);
    
	printf("pozice: %d \n", find(string, search2));
		
//	------	//

	char* search3 = NULL;
    
    printf("hledam \"%s\" v retezci %s\n", search3, string);
    
	printf("pozice: %d \n", find(string, search3));
		
//	------	//
	return 0;
}
