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
#include <time.h>
#include <stdlib.h>

int rand_255() {

	unsigned int i;
	time_t t;
	
	srand((unsigned) time(&t));
	i = rand() % 256;

	return i;
}

char* strig_gen(int x) {

	char* word;

	word = malloc(sizeof(char) * (x + 1));
	if (word == NULL) {
		exit (1);
	}

	for (int n = 0; n < x; n++) {
    	word[n] = rand() % 26 + 'a';
	}
  	word[x]='\0';

	return word;
}

int main()
{
	char* word1;
	char word2[1] = "";

	word1 = strig_gen(rand_255());
	printf("Input word1: \n%s\n", word1);
  	sort(word1);
   	printf("Sorted output: \n%s\n", word1);
   	free(word1);

   	printf("Input word2: \n%s\n", word2);
  	sort(word2);
   	printf("Sorted output: \n%s\n", word2);
   	
	return 0;
}