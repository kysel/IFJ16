/** Interpretr jazyka IFJ16
* @file semantic_check.h
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#ifndef SEMANTIC_CHECK_H_
#define SEMANTIC_CHECK_H_
#include "syntax_analysis.h"

typedef struct {
    char* description;
    int ret_code;
}Error;

typedef struct {
    Error* err;
    int size;
    int count;
} Err_list;

typedef struct {
    Syntax_context* s_ctx;
    Err_list errs;
}Sem_ctx;


/**
 * \brief Checks for semantics error in the whole AST exits with respective error code
 * \param ctx Syntax context
 */
void check_semantic(Syntax_context* ctx);

#endif
