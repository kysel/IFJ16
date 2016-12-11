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
    Function* current_func;
    Value_list* loc_stack;
    Value_list* globals;
} Inter_ctx;

/**
 * \brief Search for the function with given name in the syntax context
 * \param ctx The syntax context where to search for the function
 * \param fkName Fully qualified function name
 * \return If the function is found pointer to it, otherwise null
 */
Function* getFunc(Syntax_context* ctx, char* fkName);

/**
 * \brief Interprets AST
 * \param ctx Syntax context, that contains Main.run function
 */
void execute(Syntax_context* ctx);

#endif
