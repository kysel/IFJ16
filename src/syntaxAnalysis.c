/** Interpretr jazyka IFJ16
* @file syntaxAnalysis.c
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#include "syntaxAnalysis.h"
#include "scanner.h"
#include "ast.h"
#include "gc.h"
#include "return_codes.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void parse_class(Syntax_context* ctx);
void parse_function(Syntax_context* ctx, Data_type return_type, char* name);
Variable parse_global_variable(Syntax_context* ctx, Data_type type);

/**
 * \brief Gets fully kvalified variable name eg. "Main.foo"
 * \param ctx Syntax context
 * \param varName Variable name
 * \return fully kvalified variable name
 */
char* get_fk_name(Syntax_context* ctx, char* varName) {
    char* fkName = gc_alloc(sizeof(char)*(strlen(ctx->current_class) + strlen(varName) + 2));
    fkName[0] = 0;
    strcat(fkName, ctx->current_class);
    strcat(fkName, ".");
    strcat(fkName, varName);
    return fkName;
}

Syntax_context* init_syntax(FILE* input_file) {
    Syntax_context* ret = gc_alloc(sizeof(Syntax_context));
    ret->s_ctx = init_scanner(input_file);
    ret->global_symbols = symbol_tree_new();
    ret->local_symbols = symbol_tree_new();
    return ret;
}

Statement_collection* parse_program(Syntax_context* ctx) {
    while (peek_token(ctx->s_ctx)->type == T_KEYWORD && peek_token(ctx->s_ctx)->kw == K_CLASS) {
        parse_class(ctx);
    }

    /**
     * while token != endOfFile; parse_class(ctx);
     */
    return NULL;
}

 void parse_class(Syntax_context* ctx) {
     assert(peek_token(ctx->s_ctx)->type == T_KEYWORD && peek_token(ctx->s_ctx)->kw == K_CLASS);
     get_token(ctx->s_ctx);	//consume class token

     Ttoken* classId = check_and_get_token(ctx->s_ctx, T_ID);
     ctx->current_class = classId->c;
     check_and_get_token(ctx->s_ctx, T_BRACKET_LCURLY);

     while (check_and_peek_token(ctx->s_ctx, ~T_BRACKET_RCURLY)) {
         Ttoken* nextToken = check_and_get_token(ctx->s_ctx, T_KEYWORD);
         if (nextToken->kw == K_STATIC) {	//function or variable
             Ttoken* type = check_and_get_token(ctx->s_ctx, T_TYPE);
             Ttoken* name = check_and_get_token(ctx->s_ctx, T_ID);

             Ttoken* fnOrVar = check_and_peek_token(ctx->s_ctx, T_SEMICOLON | T_BRACKET_LROUND);
             if (fnOrVar->type == T_SEMICOLON) {
                 Variable var = parse_global_variable(ctx, type->dtype);
                 Symbol_tree_leaf* varLeaf = add_symbol(&ctx->global_symbols, get_fk_name(ctx, name->c));
                 varLeaf->type = var.type;
                 varLeaf->init_expr = var.init_expr;
             }
             else if (fnOrVar->type == T_BRACKET_LROUND)
                 parse_function(ctx, type->dtype, name->c);
         }
     }

     ctx->current_class = NULL;
 }

 void add_statement(Statement_collection collection, Statement statement) {
	 if (collection.size == collection.count) {
		 collection.size += 10;
		 collection.statements = gc_realloc(collection.statements, sizeof(Statement)*collection.size);
	 }
	 collection.statements[collection.count++] = statement;
 }

 Variable parse_global_variable(Syntax_context* ctx, Data_type type) {
     if (type == void_t) {
         fprintf(stderr, "Invalid variable type, type \'void\' cannot be used here, l. %%");
         exit(semantic_error_in_types);
     }
     Variable ret = { .type = type };
     Ttoken* nextToken = check_and_get_token(ctx->s_ctx, T_ASSIGN | T_SEMICOLON);
     if (nextToken->type == T_ASSIGN) {
         //TODO assign expression to ret.initExpression
     }
     else if (nextToken->type == T_SEMICOLON)
         ret.init_expr = NULL;
     return ret;
 }

void add_parameter(Parameter_list* list, const Func_parameter* param) {
	list->parameters = gc_realloc(list->parameters, sizeof(Func_parameter)*(list->count + 1));
	list->parameters[list->count++] = *param;
}

void parse_parameters(Syntax_context* ctx, Parameter_list* params) {
    /*
     * Parsovaní parametrů fce
     * typ Název[,|)]
     */
    while (peek_token(ctx->s_ctx)->type != T_BRACKET_RROUND) {
        Ttoken* type = check_and_get_token(ctx->s_ctx, T_TYPE);
        Ttoken* name = check_and_get_token(ctx->s_ctx, T_ID);

        Func_parameter param = {
            .name = name->c,
            .type = type->dtype
        };
        add_parameter(params, &param);

        Ttoken* nextToken = check_and_peek_token(ctx->s_ctx, T_COMMA | T_BRACKET_RROUND);
        if (nextToken->type == T_COMMA)
            get_token(ctx->s_ctx);
    }
    check_and_get_token(ctx->s_ctx, T_BRACKET_RROUND);
}

char* parse_id(Syntax_context* ctx) {
	char* name = check_and_get_token(ctx->s_ctx, T_ID)->c;
	if(peek_token(ctx->s_ctx)->type == T_DOT) {
		get_token(ctx->s_ctx); //consume '.'
		char* name2 = check_and_get_token(ctx->s_ctx, T_ID)->c;
		char* FKname = gc_alloc(strlen(name) + strlen(name2) + 2);
		strcat(FKname, name);
		strcat(FKname, ".");
		strcat(FKname, name2);
		return FKname;
	}
	return get_fk_name(ctx, name);
}

Symbol_tree_leaf* get_symbol(Syntax_context* ctx, char* key) {
	Symbol_tree_leaf* leaf = get_symbol_by_key(&ctx->local_symbols, key);
	if(leaf == NULL)
		return get_symbol_by_key(&ctx->global_symbols, key);
	return leaf;
}

void parse_assigmnent(Syntax_context* ctx, Statement_collection statements) {
	char* id = parse_id(ctx);
	Symbol_tree_leaf* symbol = get_symbol(ctx, id);
	if(symbol == NULL)
		symbol = add_symbol(&ctx->global_symbols, id);

	check_and_get_token(ctx->s_ctx, T_ASSIGN);
	Statement st = {
		.type = assigment,
		.assignment.target = symbol->id,
		/*TODO: parser here*/
	};
	add_statement(statements, st);
}

void parse_declaration(Syntax_context* ctx, Statement_collection statements) {
	Data_type type = check_and_get_token(ctx->s_ctx, T_TYPE)->dtype;
	char* name = check_and_get_token(ctx->s_ctx, T_ID)->c;

	Symbol_tree_leaf* symbol = add_symbol(&ctx->local_symbols, name);
	symbol->type = type;
	symbol->init_expr = NULL;
	Statement st = {
		.type = declaration,
		.declaration.variable.id = symbol->id,
		.declaration.variable.type = type,
		.declaration.variable.init_expr = NULL
	};

	if (peek_token(ctx->s_ctx)->type == T_SEMICOLON) {
		get_token(ctx->s_ctx);
	}else if(check_and_get_token(ctx->s_ctx, T_ASSIGN)) {
		//TODO: get expression and assign to 'init_expr' in statement, optionaly in symbol leaf
	}
	add_statement(statements, st);
}

void parse_statement(Syntax_context* ctx, Statement_collection statements) {
	Ttoken* tok;
	switch ((tok = check_and_peek_token(ctx->s_ctx, T_ID | T_KEYWORD | T_TYPE))->type) {
	case T_TYPE:
		parse_declaration(ctx, statements);
		break;
	case T_ID:
		parse_assigmnent(ctx, statements);
		break;
	case T_KEYWORD:
		switch (tok->kw) {
		case K_BREAK: break;
		case K_CLASS: break;
		case K_CONTINUE: break;
		case K_DO: break;
		case K_ELSE: break;
		case K_FALSE: break;
		case K_FOR: break;
		case K_IF: break;
		case K_RETURN: break;
		case K_STATIC: break;
		case K_TRUE: break;
		case K_WHILE: break;
		default: break;
		}break;
	default:
		fprintf(stderr, "Internal error on line %d in file %s.\n", __LINE__, __FILE__);
		exit(internal_error);
	}
}

void parse_block(Syntax_context* ctx, Statement_collection statements) {
	check_and_get_token(ctx->s_ctx, T_BRACKET_LCURLY);
	while (true) {
		token_type tType = peek_token(ctx->s_ctx)->type;
		if (tType == T_BRACKET_LCURLY)
			parse_block(ctx, statements);
		else if(tType == T_BRACKET_RCURLY)
			return;
		parse_statement(ctx, statements);
	}
}

void parse_function(Syntax_context* ctx, Data_type return_type, char* name) {
    /**
     * Vytvoření kontextu funkce
     *
     * parsování parametrů této funkce
     *
     * parsování exprešnů (těla funkce)
     *
     * ukončení kontextu funkce
     */

	Function f = {
		.name = name,
		.return_type = return_type,
		.type = user,
		.parameters.count = 0,
		.parameters.parameters = NULL };

    Symbol_tree oldSymbols = ctx->local_symbols;
    ctx->local_symbols = symbol_tree_new();

    check_and_get_token(ctx->s_ctx, T_BRACKET_LROUND);
    parse_parameters(ctx, &f.parameters);

	parse_block(ctx, f.statements);

    ctx->local_symbols = oldSymbols;
}
