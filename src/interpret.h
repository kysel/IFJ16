/** Interpretr jazyka IFJ16
* @file interpret.h
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

//todo tady by moh být interpretr astčka
#ifndef INTERPRET_H_
#define INTERPRET_H_

#include "syntaxAnalysis.h"


typedef struct {
    Data_type type;
    union {
        bool b;
        int i;
        double d;
        char* s;
    };
    bool init;
} Value;

typedef struct {
    Value* val;
    int size;
    int count;
} Value_list;

typedef struct {
    Syntax_context* s;    
    Value_list* loc_stack, global_stack;
}Inter_ctx;

void execute(Syntax_context* ctx);

#endif
