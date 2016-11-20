/** Interpretr jazyka IFJ16
* @file buildIn.h
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#include "gc.h"
#include "ial.h"
#include "return_codes.h"
#include <stdio.h>
#include <stdlib.h> // atoi

typedef struct Tstring {
	char* str;
	int len;
	int size;
} Tstring;

// https://wis.fit.vutbr.cz/FIT/st/course-files-st.php/course/IFJ-IT/projects/ifj2016.pdf 
// sekce 6

// int readInt ();
int readInt();

// double readDouble ();
double readDouble();

// String readString ();
char* readString();

// void print ( term_nebo_konkatenace );

// int length(String s);
int length(char* s);

// String substr(String s, int i, int n);
char* substr(char* s, int i, int n);

// int compare(String s1, String s2);
int compare(char* s1, char* s2);


