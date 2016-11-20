﻿/** Interpretr jazyka IFJ16
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
    Syntax_context* s;
    Value_list* loc_stack;
    Value_list* globals;
}Inter_ctx;

void execute(Syntax_context* ctx);

#endif