/** Interpretr jazyka IFJ16
* @file expr_parser.c
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#include "expr_parser.h"
#include "gc.h"

//Inicializácia (prázdneho) zásobníka
void stackInit (t_Stack* s) {
   s->arr = NULL; 
   s->stack_size = 0;
   s->top_element = -1;
   s->top_terminal = -1;
}

int stackEmpty (const t_Stack* s) {
   return (s->top_element == -1) ? 1:0;
}

int stackFull (const t_Stack* s) {
   return (s->top_element == s->stack_size - 1) ? 1:0; 
}

//Rozšírenie zásobníka o 5 prvkov
void stackExpand (t_Stack* s) {
   void *p = gc_realloc(s->arr, (s->stack_size + 5) * sizeof(t_Element)); 

   if (p){
      s->arr = p; 
      s->stack_size += 5;
   } 
   /*else TODO MALLOC ERROR*/      
}

//Vráti index terminálu, ktorý je najbližšie vrcholu zásobníku
int findTopTerminal (t_Stack* s) {
   for (int i = s->top_element; i >= 0; i--)
      if (s->arr[i].type == EOS || s->arr[i].type == TOKEN )
         return i;
   return -1;
}

void stackSetStopBit (t_Stack* s) {
   s->arr[s->top_terminal].stop_bit = 1;
}

void stackPop (t_Stack* s) {
   if (!stackEmpty(s)){
      s->top_element -= 1;
      s->top_terminal = findTopTerminal(s);  
   }
}

//Pridanie prvku na vrchol zásobníka
void stackPush (t_Stack* s, t_Element_Type type, void * address) {
   //Ak je zasobnik prázdny, je rozšírený
   if (stackFull(s)) 
      stackExpand(s);

   s->top_element += 1;
   s->arr[s->top_element].type = type;
   s->arr[s->top_element].address = address;
   s->arr[s->top_element].stop_bit = 0;
   if (type == EOS || type == TOKEN )
      s->top_terminal = s->top_element;
     
}

/***
 *        ____  ___    ____  _____ __________ 
 *       / __ \/   |  / __ \/ ___// ____/ __ \
 *      / /_/ / /| | / /_/ /\__ \/ __/ / /_/ /
 *     / ____/ ___ |/ _, _/___/ / /___/ _, _/ 
 *    /_/   /_/  |_/_/ |_|/____/_____/_/ |_|  
 * 
 */

/*TODO
 node * parseExppression() {
   do {
      switch (preccedense_tab[][])
         case "=": break;   
         case "<": break;
         case ">": break;
         default: break;
   } while ();
}*/