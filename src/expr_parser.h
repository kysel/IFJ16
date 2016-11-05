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
      EOS, TOKEN, EXPRESSION
   } t_Element_Type;

//Prvok zásobníka
typedef struct {
   t_Element_Type type;
   void *address;
   int stop_bit;
} t_Element;

//Zásobník
typedef struct {
   t_Element *arr;
   int stack_size;
   int top_element;
   int top_token; 
} t_Stack;


//Precedenčná tabuľka
static const unsigned char precedence_tab[15][15] = {
// 'M' - more    'L' - less
// 'E' - equal   'O' - error
//TODO nedokoncena bodka
//   +    -    *    /    <    >    <=   >=   ==   !=   (    )    .    id   $  
   {'M', 'M', 'L', 'L', 'M', 'M', 'M', 'M', 'M', 'M', 'L', 'M', 'L', 'L', 'M'}, // +
   {'M', 'M', 'L', 'L', 'M', 'M', 'M', 'M', 'M', 'M', 'L', 'M', 'L', 'L', 'M'}, // -
   {'M', 'M', 'M', 'M', 'M', 'M', 'M', 'M', 'M', 'M', 'L', 'M', 'L', 'L', 'M'}, // *
   {'M', 'M', 'M', 'M', 'M', 'M', 'M', 'M', 'M', 'M', 'L', 'M', 'L', 'L', 'M'}, // /
   {'L', 'L', 'L', 'L', 'M', 'M', 'M', 'M', 'M', 'M', 'L', 'M', 'L', 'L', 'M'}, // <
   {'L', 'L', 'L', 'L', 'M', 'M', 'M', 'M', 'M', 'M', 'L', 'M', 'L', 'L', 'M'}, // >
   {'L', 'L', 'L', 'L', 'M', 'M', 'M', 'M', 'M', 'M', 'L', 'M', 'L', 'L', 'M'}, // <=
   {'L', 'L', 'L', 'L', 'M', 'M', 'M', 'M', 'M', 'M', 'L', 'M', 'L', 'L', 'M'}, // >=
   {'L', 'L', 'L', 'L', 'M', 'M', 'M', 'M', 'M', 'M', 'L', 'M', 'L', 'L', 'M'}, // ==
   {'L', 'L', 'L', 'L', 'M', 'M', 'M', 'M', 'M', 'M', 'L', 'M', 'L', 'L', 'M'}, // !=
   {'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'E', ' ', 'L', 'O'}, // (
   {'M', 'M', 'M', 'M', 'M', 'M', 'M', 'M', 'M', 'M', 'O', 'M', ' ', 'O', 'M'}, // )
   {'M', 'M', 'M', 'M', 'M', 'M', 'M', 'M', 'M', 'M', ' ', ' ', 'M', 'L', 'M'}, // .
   {'M', 'M', 'M', 'M', 'M', 'M', 'M', 'M', 'M', 'M', 'O', 'M', 'M', 'O', 'M'}, // id
   {'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'O', 'L', 'L', 'O'}  // $
};

Expression* parseExpression(Tinit *scanner);