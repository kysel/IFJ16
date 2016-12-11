/** Interpretr jazyka IFJ16
* @file expr_parser.h
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#ifndef EXPR_PARSER_H_
#define EXPR_PARSER_H_
#include "scanner.h"
#include "ast.h"
#include "ial.h"

typedef struct {
    Symbol_tree* global_tab;
    Symbol_tree* local_tab;
    char* class_name;
    bool inside_func;
} t_Expr_Parser_Init;

// Datový typ prvku na zásobníku
typedef enum {
    EOS,
    TOKEN,
    EXPRESSION
} t_Element_Type;

//Prvok zásobníka
typedef struct {
    t_Element_Type type;
    void* address;
    int stop_bit;
} t_Element;

//Zásobník
typedef struct {
    t_Element* arr;
    int stack_size;
    int top_element;
    int top_token;
} t_Stack;


//Precedenčná tabuľka
static const unsigned char precedence_tab[20][20] = {
// 'M' - more    'L' - less
// 'E' - equal   'O' - error
// 'F' - function call

//            0    1    2    3      4    5    6    7    8    9      10   11   12     13   14   15     16   17   18   19
//            +    -    *    /      ==   !=   <    >    <=   >=     !    &&   ||     -    ++   --     (    )    id   $
/* 0  +  */ {'>', '>', '<', '<',   '>', '>', '>', '>', '>', '>',   '<', '>', '>',   '<', '<', '<',   '<', '>', '<', '>'},
/* 1  -  */ {'>', '>', '<', '<',   '>', '>', '>', '>', '>', '>',   '<', '>', '>',   '<', '<', '<',   '<', '>', '<', '>'},
/* 2  *  */ {'>', '>', '>', '>',   '>', '>', '>', '>', '>', '>',   '<', '>', '>',   '<', '<', '<',   '<', '>', '<', '>'},
/* 3  /  */ {'>', '>', '>', '>',   '>', '>', '>', '>', '>', '>',   '<', '>', '>',   '<', '<', '<',   '<', '>', '<', '>'},

/* 4  == */ {'<', '<', '<', '<',   '>', '>', '<', '<', '<', '<',   '<', '>', '>',   '<', '<', '<',   '<', '>', '<', '>'},
/* 5  != */ {'<', '<', '<', '<',   '>', '>', '<', '<', '<', '<',   '<', '>', '>',   '<', '<', '<',   '<', '>', '<', '>'},
/* 6  <  */ {'<', '<', '<', '<',   '>', '>', '>', '>', '>', '>',   '<', '>', '>',   '<', '<', '<',   '<', '>', '<', '>'},
/* 7  >  */ {'<', '<', '<', '<',   '>', '>', '>', '>', '>', '>',   '<', '>', '>',   '<', '<', '<',   '<', '>', '<', '>'},
/* 8  <= */ {'<', '<', '<', '<',   '>', '>', '>', '>', '>', '>',   '<', '>', '>',   '<', '<', '<',   '<', '>', '<', '>'},
/* 9  >= */ {'<', '<', '<', '<',   '>', '>', '>', '>', '>', '>',   '<', '>', '>',   '<', '<', '<',   '<', '>', '<', '>'},

/* 10 !  */ {'>', '>', '>', '>',   '>', '>', '>', '>', '>', '>',   '<', '>', '>',   '<', '<', '<',   '<', '>', '<', '>'},
/* 11 && */ {'<', '<', '<', '<',   '<', '<', '<', '<', '<', '<',   '<', '>', '>',   '<', '<', '<',   '<', '>', '<', '>'},
/* 12 || */ {'<', '<', '<', '<',   '<', '<', '<', '<', '<', '<',   '<', '<', '>',   '<', '<', '<',   '<', '>', '<', '>'},

/* 13 -  */ {'>', '>', '>', '>',   '>', '>', '>', '>', '>', '>',   '<', '>', '>',   '<', '<', '<',   '<', '>', '<', '>'},
/* 14 ++ */ {'>', '>', '>', '>',   '>', '>', '>', '>', '>', '>',   '<', '>', '>',   '<', '<', '<',   '<', '>', '<', '>'},
/* 15 -- */ {'>', '>', '>', '>',   '>', '>', '>', '>', '>', '>',   '<', '>', '>',   '<', '<', '<',   '<', '>', '<', '>'},

/* 16 (  */ {'<', '<', '<', '<',   '<', '<', '<', '<', '<', '<',   '<', '<', '<',   '<', '<', '<',   '<', '=', '<', 'E'},
/* 17 )  */ {'>', '>', '>', '>',   '>', '>', '>', '>', '>', '>',   '>', '>', '>',   '>', '>', '>',   'E', '>', 'E', '>'},
/* 18 id */ {'>', '>', '>', '>',   '>', '>', '>', '>', '>', '>',   'E', '>', '>',   'E', '>', '>',   'F', '>', 'E', '>'},
/* 19 $  */ {'<', '<', '<', '<',   '<', '<', '<', '<', '<', '<',   '<', '<', '<',   '<', '<', '<',   '<', 'E', '<', 'E'}
};

/**
 * \brief Initializes expression parser
 * \param global_tab Pointer to the global symbol table
 * \param local_tab Pointer to the current function's symbol table
 * \param class_name Current class name (used for generation fully qualified IDs)
 * \return Expression parser struct
 */
t_Expr_Parser_Init* ExprParserInit(Symbol_tree* global_tab, Symbol_tree* local_tab, char* class_name);

/**
 * \brief Parses expression 
 * \param parser_ctx Parser struct obtained from \see{ExprParserInit}
 * \param scanner Pointer to the scanner structure
 * \return Parsed expression
 */
Expression* parseExpression(t_Expr_Parser_Init* parser_ctx, Tinit* scanner);

#endif
