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
   s->top_token = -1;
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
   s->arr[s->top_token].stop_bit = 1;
}

void stackPop (t_Stack* s) {
   if (!stackEmpty(s)){
      s->top_element -= 1;
      s->top_token = findTopTerminal(s);  
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
      s->top_token = s->top_element;
     
}

//
int token2TabIndex(Ttoken * token) {
   //Jediný "token", ktorý má adresu nula je EOS - End of Stack - Dno zásobníka
   if (token == NULL)
      return 13;
   switch(token->type) {
      case T_ADD: return 0;
      case T_SUB: return 1;
      case T_MUL: return 2;
      case T_DIV: return 3;
      case T_LOWER: return 4;
      case T_GREATER: return 5;
      case T_LOWER_EQUAL: return 6;
      case T_GREATER_EQUAL: return 7;
      case T_BOOL_EQUAL: return 8;
      case T_NOT_EQUAL: return 9;
      case T_BRACKET_LROUND: return 10;
      case T_BRACKET_RROUND: return 11;
      case T_ID: return 12;
      case T_EOF: case T_COMMA: return 13;
      default: return -1;//TODO error;
   }
}

/***
 *        ____  ___    ____  _____ __________ 
 *       / __ \/   |  / __ \/ ___// ____/ __ \
 *      / /_/ / /| | / /_/ /\__ \/ __/ / /_/ /
 *     / ____/ ___ |/ _, _/___/ / /___/ _, _/ 
 *    /_/   /_/  |_/_/ |_|/____/_____/_/ |_|  
 * 
 */

void parseExppression(FILE *file) {
   t_Stack* stack = gc_alloc(sizeof(t_Stack));
   stackInit(stack);
   stackPush(stack, EOS, NULL);

   Ttoken *a = stack->arr[stack->top_token].address;
   Ttoken *b = get_token(file);
   
   do {
      switch(precedence_tab[token2TabIndex(a)][token2TabIndex(b)]){
         case 'E':
            stackPush(stack, TOKEN, b);
            b = get_token(file);
            break;
         
         case 'L': 
            stackSetStopBit(stack);
            stackPush(stack, TOKEN, b);
            b = get_token(file);
            break;
         
         case 'M':
            /* TODO 
            stackApplyRule(s); 
            */
            break;
         
         default:
            //TODO ERROR
            break;   
         }
      
      //Aktualizuje hodnotu top_token
      a = stack->arr[stack->top_token].address;
   } while (a != NULL || /*TODO*/b->type != EOF);
}