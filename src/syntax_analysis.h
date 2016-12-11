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
#include "expr_parser.h"

typedef struct {
    Symbol_tree global_symbols, local_symbols, classes;
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


/**
 * \brief Parses function call
 * \param exprCtx Expression parser context
 * \param scanner Scanner context
 * \param id Function id
 * \return Expression that contains parsed function call
 */
Expression* parse_f_call(t_Expr_Parser_Init* exprCtx, Tinit* scanner, char* id);

/**
 * \brief Initializes suntax parser
 * \param input_file Handle to the opened file, that has to be parsed
 * \return Initialized syntax analyzer context
 */
Syntax_context* init_syntax(FILE* input_file);

/**
 * \brief Parses input file and generates AST in syntax context
 * \param ctx Syntax context
 */
void parse_program(Syntax_context* ctx);
#endif
