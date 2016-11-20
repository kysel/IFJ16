/** Interpretr jazyka IFJ16
* @file syntax_analysis.h
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#ifndef SYNTAXANALYSIS_H_
#define SYNTAXANALYSIS_H_
#include "scanner.h"
#include "ast.h"
#include "ial.h"
#include "expr_parser.h"

typedef struct {
    Symbol_tree global_symbols, local_symbols;
    Tinit* s_ctx;
    char* current_class;
    Function_list functions;
    t_Expr_Parser_Init* expCtx;
    int depth;
    int globals;
} Syntax_context;

typedef struct {
    bool fullQ;
    char* class;
    char* name;
    char* full;
    Ttoken* nameTok;
} Parsed_id;

Statement* parse_f_call(t_Expr_Parser_Init* exprCtx, Tinit* scanner, char* id);
Syntax_context* init_syntax(FILE* input_file);
void parse_program(Syntax_context* ctx);
#endif
