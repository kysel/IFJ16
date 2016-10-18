/** Interpretr jazyka IFJ16
* @file expr_parser.h
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#include <stdio.h>
#include "scanner.h"

// Datový typ prvku na zásobníku
typedef enum {
      EOS, TOKEN, NODE, STOP
   } t_Element_Type;

// Hodnota prvku na zásobníku
typedef union {
   //TODO 
   void *token_ptr;
   void *node_ptr;
} t_Element_Value;

//Prvok zásobníka
typedef struct {
   t_Element_Type type;
   t_Element_Value value;
} t_Element;

//Zásobník
typedef struct {
   t_Element *arr;
   int stack_size;
   int top_element;
   int top_terminal; 
} t_Stack;

//Precedenčná tabuľka


void stackInit ( t_Stack* s );
int stackEmpty ( const t_Stack* s );
void stackTop ( const t_Stack* s, char* c );
void stackPop ( t_Stack* s );
void stackPush ( t_Stack* s, t_Element element );