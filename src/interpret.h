/** Interpretr jazyka IFJ16
* @file interpret.h
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#ifndef INTERPRET_H_
#define INTERPRET_H_
#include "syntax_analysis.h"

typedef struct {
    Syntax_context* s;
    Value_list* loc_stack;
    Value_list* globals;
} Inter_ctx;

Function* getFunc(Syntax_context* ctx, char* fkName);
void execute(Syntax_context* ctx);

#endif
