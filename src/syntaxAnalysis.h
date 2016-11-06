/** Interpretr jazyka IFJ16
* @file syntaxAnalysis.h
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#ifndef SYNTAXANALYSIS_H_
#define SYNTAXANALYSIS_H_

#include "ial.h"
#include "ast.h"
#include "scanner.h"

typedef struct {
	Symbol_tree global_symbols, local_symbols;
	Tinit* s_ctx;
	char* current_class;
	Function_list functions;
}Syntax_context;

Syntax_context* init_syntax(FILE* input_file);
Statement_collection* parse_program(Syntax_context* ctx);
char* parse_id(Syntax_context* ctx);
#endif
