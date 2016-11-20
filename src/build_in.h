/** Interpretr jazyka IFJ16
* @file build_in.h
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#ifndef BUILDIN_H_
#define BUILDIN_H_
#include "interpret.h"

typedef struct Tstring {
    char* str;
    int len;
    int size;
} Tstring;

// int readInt ();
Value readInt(Value_list vals);

// double readDouble ();
Value readDouble(Value_list vals);

// String readString ();
Value readString(Value_list vals);

// void print ( term_nebo_konkatenace );
Value print(Value_list vals);

// int length(String s);
Value length(Value_list vals);

// String substr(String s, int i, int n);
Value substr(Value_list vals);

// int compare(String s1, String s2);
Value compare(Value_list vals);

// int find(String s, String search);
Value findBI(Value_list vals);

// String sort(String s);
Value sortBI(Value_list vals);

#endif
