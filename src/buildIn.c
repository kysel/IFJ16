/** Interpretr jazyka IFJ16
* @file buildIn.h
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#include "gc.h"
#include "buildIn.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// https://wis.fit.vutbr.cz/FIT/st/course-files-st.php/course/IFJ-IT/projects/ifj2016.pdf 
// sekce 6

// int readInt ();
int readInt() {

	Tstring * r_int;	// deklarace struktury
	int i = 0;			// delka, nasledne vysledne cislo
//	int sig = 1;		// znamenko vysledneho cisla
//	int sig_det = 0; 	// detekce znamenka
	int err = 0;		// detekce erroru (podminky retezce)
	char c;				// nacteny znak

	gc_init();			// garbage collector

	r_int = gc_alloc(sizeof(struct Tstring));				// alokace mista pro strukturu
	r_int -> str = gc_alloc(sizeof(char) * INC_STRLEN);		// alokace mista pro string

	r_int -> size = INC_STRLEN;		// aktualni alokovana velikost
	r_int -> len = 0;				// delka retezce

	c = getchar();					// cteni prvniho znaku
/*	if (c == '-' || c == '+') {		// kontrola na znamenko
		if (c == '-') {
			sig = -1;
			sig_det = 1;
		}
		if (c == '+') {
			sig = 1;
			sig_det = 1;
		}
		c = getchar();	// cte se nasledujici znak
	}
*/
	while (c >= '0' && c <= '9') {	// cteni cisla
		if (r_int -> len == r_int -> size) {		// pokud je zaplnen alokovany prostor
			r_int -> str = gc_realloc(r_int -> str, r_int -> size + (sizeof(char) * INC_STRLEN));
			r_int -> size += INC_STRLEN;
		}
		r_int -> str[i] = c;
		i++;
		r_int -> len = i;
		c = getchar();
	}
    if (c == '\n' || c == EOF)		// pokud je ukonceno cteni
    {
    	if (r_int -> len > 0 /*|| sig_det == 0*/) {		// pokud se nacetlo cislo
    		i = atoi(r_int -> str);
    		i = /*sig **/ i;
    	} else {
    		err = 1;
    	}
    }
    if (c != '\n' && c != EOF)		// pokud byl zaznamenan jiny nepripustny znak
    {
    	err = 1;
    }
    if (err == 0) {
    	return i;
    }
	else {
		fprintf(stderr, "Input error\n");
		exit(runtime_input_error);
	}
}

// double readDouble ();
double readDouble() {

	Tstring * r_dbl;	// deklarace struktury
	int i = 0;			// delka
	double d = 0;	// vysledne desetinne cislo
	char *ptr;			// spatny retezec
	int err = 0;		// detekce erroru (podminky retezce)
	char c;				// nacteny znak

	gc_init();			// garbage collector

	r_dbl = gc_alloc(sizeof(struct Tstring));				// alokace mista pro strukturu
	r_dbl -> str = gc_alloc(sizeof(char) * INC_STRLEN);		// alokace mista pro string

	r_dbl -> size = INC_STRLEN;		// prvotni alokovana velikost
	r_dbl -> len = 0;				// delka retezce

	c = getchar();					// cteni prvniho znaku
	if (c == '\n') {				// pokud je vstupem prazdny retezec
		return 0;
	}
	if (/*c != '-' && c != '+' && */(c < '0' || c > '9')) {		// kontrola na spatny prvni znak
		err = 1;
	}
	while ((c >= '0' && c <= '9') || c == '.' || c == 'e' || c == 'E' /* || c == '+' || c == '-'*/) {	// cteni stringu tvoreneho povolenými znaky
		if (r_dbl -> len == r_dbl -> size) {		// pokud je zaplnen alokovany prostor
			r_dbl -> str = gc_realloc(r_dbl -> str, r_dbl -> size + (sizeof(char) * INC_STRLEN));
			r_dbl -> size += INC_STRLEN;
		}
		r_dbl -> str[i] = c;
		i++;
		r_dbl -> len = i;
		c = getchar();
	}
	while (c != '\n' && c != EOF)	{	// pokud byl zaznamenan jiny nepripustny znak
    	err = 1;
    	c = getchar();
    }
	if (c == '\n' || c == EOF) {
		d = strtod(r_dbl -> str, &ptr);
		if (err == 1 || *ptr != '\0') {
			fprintf(stderr, "Input error\n");
			exit(runtime_input_error);
		}
	}
	return d;
}

// String readString ();
char* readString() {

	Tstring * r_str;	// deklarace struktury
	int i = 0;			// delka, nasledne vysledne cislo
	char c;				// nacteny znak

	gc_init();			// garbage collector

	r_str = gc_alloc(sizeof(struct Tstring));		// alokace mista pro strukturu
	r_str -> str = gc_alloc(sizeof(char) * INC_STRLEN);		// alokace mista pro string

	r_str -> size = INC_STRLEN;		// aktualni alokovana velikost
	r_str -> len = 0;				// delka retezce

	c = getchar();		// cteni prvniho znaku
	while (c != '\n' && c != EOF) {
		if (r_str -> len == r_str -> size) {		// pokud je zaplnen alokovany prostor
			r_str -> str = gc_realloc(r_str -> str, r_str -> size + (sizeof(char) * INC_STRLEN));
			r_str -> size += INC_STRLEN;
		}
		r_str -> str[i] = c;
		i++;
		r_str -> len = i;
		c = getchar();
	}
	if (r_str -> len == r_str -> size) {		// pokud je zaplnen alokovany prostor
			r_str -> str = gc_realloc(r_str -> str, r_str -> size + sizeof(char));
			r_str -> size += 1;
	}
	r_str -> str[i] = '\0';
	return r_str -> str;
}

// void print ( term_nebo_konkatenace );

// int length(String s);
int length(char* s) {
	return strlen(s);
}
// String substr(String s, int i, int n);
char* substr(char* s, int i, int n) {

	gc_init();

	int len = strlen(s);

	if((i + n) > len) {
		fprintf(stderr, "Substring error\n");
		exit(runtime_error);
	}
//	strncpy(dest, src + beginIndex, endIndex - beginIndex);
	return s;
}

// int compare(String s1, String s2);
int compare(char* s1, char* s2) {
	return strcmp(s1,s2);
}
