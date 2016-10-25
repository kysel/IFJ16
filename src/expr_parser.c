      /** Interpretr jazyka IFJ16
   * @file expr_parser.c
   * @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
   * @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
   * @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
   * @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
   * @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
   */

   #include <stdlib.h>
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
      t_Element *p = gc_realloc(s->arr, (s->stack_size + 5) * sizeof(t_Element)); 

      s->arr = p; 
      s->stack_size += 5;
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
      if (stackFull(s)){
         stackExpand(s);
      }
      
      s->top_element += 1;
      s->arr[s->top_element].type = type;
      s->arr[s->top_element].address = address;
      s->arr[s->top_element].stop_bit = 0;
      if (type == EOS || type == TOKEN )
         s->top_token = s->top_element;
   }

   void stackApplyRule(t_Stack* s) {
      //TODO Pravidlo E -> i
      if ((s->top_element > 0) && (s->arr[s->top_element - 1].stop_bit)) {
         stackPop(s);
      }
      //TODO Pravidlá E -> E _ E
      else if ((s->top_element > 2) && (s->arr[s->top_element - 3].stop_bit) && !(s->arr[s->top_element - 2].stop_bit) && !(s->arr[s->top_element - 1].stop_bit) && !(s->arr[s->top_element].stop_bit)) {
         Ttoken *token = s->arr[s->top_element - 2].address;
         switch(token->type) {
            case T_ADD: 
               break;
            case T_SUB:
               break;
            case T_MUL:
               break;
            case T_DIV: 
               break;
            case T_LOWER: 
               break;
            case T_GREATER: 
               break;
            case T_LOWER_EQUAL: 
               break;
            case T_GREATER_EQUAL: 
               break;
            case T_BOOL_EQUAL: 
               break;
            case T_NOT_EQUAL: 
               break;
            default:
               //TODO ERROR
               break;
         }
         stackPop(s);
         stackPop(s);
         stackPop(s);   
      }
      else {
         //TODO ERROR
      }

      s->arr[s->top_element].stop_bit = 0;
      stackPush(s, NODE, NULL);
   }

   //
   int terminal2TabIndex(void * terminal) {
      //Jediný terminál, ktorý má adresu nula je EOS - End of Stack - Dno zásobníka
      if (terminal == NULL)
         return 13;

      //Ak má terminál adresu rôznu od NULL, ide o token
      //Konverzia pointeru na terminál na pointer na token 
      Ttoken *token = terminal;
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
         case T_ID: case T_INT: return 12;
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

   void printStack(t_Stack *s) {
      Ttoken *token;
      for (int i = 0; i <= s->top_element; i++) {
         switch(s->arr[i].type) {
            case EOS: printf("$"); break;
            case NODE: printf("A"); break;
            case TOKEN: 
               token = s->arr[i].address; 
               if (token->type == T_ADD)
                  printf("+");
               else if (token->type == T_SUB)
                  printf("-");
               else if (token->type == T_MUL)
                  printf("*");
               else if (token->type == T_DIV)
                  printf("/");
               else if (token->type == T_INT)
                  printf("1");
               else if (token->type == T_BRACKET_LROUND)
                  printf("(");
               else if (token->type == T_BRACKET_RROUND)
                  printf(")");
               else if (token->type == T_BOOL_EQUAL)
                  printf("==");
               else
                  printf("x");
               break;
         };
         if (s->arr[i].stop_bit)
            printf("<");
      }
      printf("\n");
   }

   void parseExppression(FILE *file) {
      t_Stack* stack = gc_alloc(sizeof(t_Stack));
      stackInit(stack);
      stackPush(stack, EOS, NULL);
      
      void *a = stack->arr[stack->top_token].address;
      Ttoken *b = get_token(file);

      do {   
         switch(precedence_tab[terminal2TabIndex(a)][terminal2TabIndex(b)]){
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
               stackApplyRule(stack); 
               break;
            
            default:
               //TODO Treba uvoľniť pamäť, vypísať chybové hlásenie a až potom exit. Skontroluj všade!
               exit(1);
               break;   
            }

         printStack(stack);
         a = stack->arr[stack->top_token].address;
      } while (a != NULL || b->type != T_EOF);
   }