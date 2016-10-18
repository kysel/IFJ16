/** Interpretr jazyka IFJ16
* @file main.c
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/
#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "gc.h"
#include "expr_parser.h"


int main()
{
    gc_init();

    t_Stack* zasobnik = gc_alloc(sizeof(t_Stack));
    stackInit(zasobnik);

    stackPush(zasobnik, EOS, 2133132);
    stackPush(zasobnik, TOKEN, 12);
    stackPush(zasobnik, TOKEN, 12);
    stackPush(zasobnik, TOKEN, 12);
    stackPush(zasobnik, TOKEN, 12);

    for (int i = 0; i <= zasobnik->top_element; i++)
        printf("Type: %d, Address: %09d, StopBit: %d\n",zasobnik->arr[i].type,zasobnik->arr[i].address, zasobnik->arr[i].stop_bit);
    printf("\n");
    stackSetStopBit(zasobnik);
    
    for (int i = 0; i <= zasobnik->top_element; i++)
        printf("Type: %d, Address: %09d, StopBit: %d\n",zasobnik->arr[i].type,zasobnik->arr[i].address, zasobnik->arr[i].stop_bit);
    printf("\n");
    stackPush(zasobnik, TOKEN, 12);
    stackPush(zasobnik, TOKEN, 12);
    stackPush(zasobnik, TOKEN, 12);

    for (int i = 0; i <= zasobnik->top_element; i++)
        printf("Type: %d, Address: %09d, StopBit: %d\n",zasobnik->arr[i].type,zasobnik->arr[i].address, zasobnik->arr[i].stop_bit);
    printf("\n");
    while(zasobnik->arr[zasobnik->top_element].stop_bit != 1)
        stackPop(zasobnik);

    zasobnik->arr[zasobnik->top_terminal].stop_bit = 0;


    for (int i = 0; i <= zasobnik->top_element; i++)
        printf("Type: %d, Address: %09d, StopBit: %d\n",zasobnik->arr[i].type,zasobnik->arr[i].address, zasobnik->arr[i].stop_bit);  
    
    return 0;
}
