/** Interpretr jazyka IFJ16
* @file syntaxAnalysis.c
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "gc.h"
#include "return_codes.h"
#include "scanner.h"
#include "ast.h"
#include "expr_parser.h"
#include "syntaxAnalysis.h"

void parse_class(Syntax_context* ctx);
void parse_function(Syntax_context* ctx, Data_type return_type, char* name);
void parse_statement(Syntax_context* ctx, Statement_collection* statements);
void parse_block(Syntax_context* ctx, Statement_collection* statements);
Variable parse_global_variable(Syntax_context* ctx, Data_type type);


/**
 * \brief Gets fully kvalified variable name eg. "Main.foo"
 * \param ctx Syntax context
 * \param varName Variable name
 * \return fully kvalified variable name
 */
char* get_fk_name(const char* className, const char* varName) {
    char* fkName = gc_alloc(sizeof(char)*(strlen(className) + strlen(varName) + 2));
    fkName[0] = 0;
    strcat(fkName, className);
    strcat(fkName, ".");
    strcat(fkName, varName);
    return fkName;
}

Syntax_context* init_syntax(FILE* input_file) {
    Syntax_context* ret = gc_alloc(sizeof(Syntax_context));
    ret->s_ctx = init_scanner(input_file);
    ret->global_symbols = symbol_tree_new();
    ret->local_symbols = symbol_tree_new();
    ret->functions.count = 0;
    ret->functions.size = 0;
    ret->functions.items = 0;
    ret->expCtx = ExprParserInit(&ret->global_symbols, &ret->local_symbols, "");
    return ret;
}

void parse_program(Syntax_context* ctx) {
    while (peek_token(ctx->s_ctx)->type == T_KEYWORD && peek_token(ctx->s_ctx)->kw == K_CLASS) {
        parse_class(ctx);
    }

    /**
     * while token != endOfFile; parse_class(ctx);
     */
    //return NULL;
}

Parsed_id parse_id(Tinit* scanner, const char* currentClass) {
    Parsed_id ret = { .fullQ = false };
    Ttoken* nameTok = check_and_get_token(scanner, T_ID);
    ret.class = gc_alloc(sizeof(char)*strlen(currentClass) +1);
    ret.class = strcpy(ret.class, currentClass);
    ret.name = nameTok->c;
    ret.full = nameTok->c;
    ret.nameTok = nameTok;
    if (peek_token(scanner)->type == T_DOT) {
        get_token(scanner); //consume '.'
        char* name2 = check_and_get_token(scanner, T_ID)->c;
        char* FKname = gc_alloc(strlen(nameTok->c) + strlen(name2) + 2);
        FKname[0] = 0;
        strcat(FKname, nameTok->c);
        strcat(FKname, ".");
        strcat(FKname, name2);
        ret.fullQ = true;
        ret.class = nameTok->c;
        ret.name = name2;
        ret.full = FKname;
    }
    return ret;
}

 void parse_class(Syntax_context* ctx) {
     assert(peek_token(ctx->s_ctx)->type == T_KEYWORD && peek_token(ctx->s_ctx)->kw == K_CLASS);
     get_token(ctx->s_ctx);	//consume class token

     Ttoken* classId = check_and_get_token(ctx->s_ctx, T_ID);
     ctx->current_class = classId->c;
     check_and_get_token(ctx->s_ctx, T_BRACKET_LCURLY);
     ctx->expCtx->class_name = ctx->current_class;

     while (peek_token(ctx->s_ctx)->type != T_BRACKET_RCURLY) {
         Ttoken* nextToken = check_and_get_token(ctx->s_ctx, T_KEYWORD);
         if (nextToken->kw == K_STATIC) {	//function or variable
             Ttoken* type = check_and_get_token(ctx->s_ctx, T_TYPE);
             Parsed_id name = parse_id(ctx->s_ctx, ctx->current_class);
             if (name.fullQ) {
                 fprintf(stderr, "Fully qualified name not allowed here, line %lld", name.nameTok->line);
                 exit(98); //TODO vymyslet error code
             }

             Ttoken* fnOrVar = check_and_peek_token(ctx->s_ctx, T_SEMICOLON | T_BRACKET_LROUND);
             if (fnOrVar->type == T_SEMICOLON) {
                 Variable var = parse_global_variable(ctx, type->dtype);
                 Symbol_tree_leaf* varLeaf = add_symbol(&ctx->global_symbols, name.full);
                 varLeaf->type = var.type;
                 varLeaf->init_expr = var.init_expr;
             }
             else if (fnOrVar->type == T_BRACKET_LROUND)
                 parse_function(ctx, type->dtype, name.name);
         }
     }
     check_and_get_token(ctx->s_ctx, T_BRACKET_RCURLY);
     ctx->current_class = NULL;
     ctx->expCtx->class_name = ctx->current_class;
 }

 void add_statement(Statement_collection* collection, Statement statement) {
     if (collection->size == collection->count) {
         collection->size += 10;
         collection->statements = gc_realloc(collection->statements, sizeof(Statement)*collection->size);
     }
     collection->statements[collection->count++] = statement;
 }

void add_parameter(Parameter_list collection, Expression expr) {
    if (collection.size == collection.count) {
        collection.size += 10;
        collection.parameters = gc_realloc(collection.parameters, sizeof(Func_parameter)*collection.size);
    }
    collection.parameters[collection.count].type = 0;
    collection.parameters[collection.count].name = 0;
    collection.parameters[collection.count++].value = expr;
}

 Variable parse_global_variable(Syntax_context* ctx, Data_type type) {
     if (type == void_t) {
         fprintf(stderr, "Invalid variable type, type \'void\' cannot be used here, l. %%");
         exit(semantic_error_in_types);
     }
     Variable ret = { .type = type };
     Ttoken* nextToken = check_and_get_token(ctx->s_ctx, T_ASSIGN | T_SEMICOLON);

     if (nextToken->type == T_ASSIGN)
         ret.init_expr = parseExpression(ctx->expCtx, ctx->s_ctx);
     else if (nextToken->type == T_SEMICOLON)
         ret.init_expr = NULL;
     return ret;
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
        params->parameters = gc_realloc(params->parameters, sizeof(Func_parameter)*(params->count + 1));
        params->parameters[params->count++] = param;

        Symbol_tree_leaf* symbol = add_symbol(&ctx->local_symbols, param.name);
        symbol->type = param.type;

        Ttoken* nextToken = check_and_peek_token(ctx->s_ctx, T_COMMA | T_BRACKET_RROUND);
        if (nextToken->type == T_COMMA)
            get_token(ctx->s_ctx);
    }
    check_and_get_token(ctx->s_ctx, T_BRACKET_RROUND);
}


/*
char* parse_id(Tinit* scanner, char* currentClass) {
    char* name = check_and_get_token(scanner, T_ID)->c;
    if (peek_token(scanner)->type == T_DOT) {
        get_token(scanner); //consume '.'
        char* name2 = check_and_get_token(scanner, T_ID)->c;
        char* FKname = gc_alloc(strlen(name) + strlen(name2) + 2);
        strcat(FKname, name);
        strcat(FKname, ".");
        strcat(FKname, name2);
        return FKname;
    }
    return get_fk_name(currentClass, name);
}*/

Symbol_tree_leaf* get_symbol(Syntax_context* ctx, char* key) {
    Symbol_tree_leaf* leaf = get_symbol_by_key(&ctx->local_symbols, key);
    if (leaf == NULL)
        return get_symbol_by_key(&ctx->global_symbols, key);
    return leaf;
}

void parse_assigmnent(Syntax_context* ctx, Statement_collection* statements, Parsed_id id) {
    Symbol_tree_leaf* symbol = get_symbol(ctx, id.full);
    if (id.fullQ == false && symbol == NULL)
        //not fully qualified class variable
        id.full = get_fk_name(id.class, id.name);
    symbol = get_symbol(ctx, id.full);
    if (symbol == NULL)
        symbol = add_symbol(&ctx->global_symbols, id.full);

    check_and_get_token(ctx->s_ctx, T_ASSIGN);
    Statement st = {
        .type = assigment,
        .assignment.target = symbol->id,
    };
    //TODO use return value as exression in statement
    parseExpression(ctx->expCtx, ctx->s_ctx);
    add_statement(statements, st);
    check_and_get_token(ctx->s_ctx, T_SEMICOLON);
}

void parse_declaration(Syntax_context* ctx, Statement_collection* statements) {
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
    }
    else if (check_and_get_token(ctx->s_ctx, T_ASSIGN)) {
        //TODO: get expression and assign to 'init_expr' in statement, optionaly in symbol leaf
        parseExpression(ctx->expCtx, ctx->s_ctx);
        check_and_get_token(ctx->s_ctx, T_SEMICOLON);
    }
    add_statement(statements, st);
}

void parse_if(Syntax_context* ctx, Statement_collection* statements) {
    Statement st = {
        .type = condition,
        .condition.caseTrue.size = 0,
        .condition.caseFalse.size = 0
    };

    check_and_get_keyword(ctx->s_ctx, K_IF);
    check_and_get_token(ctx->s_ctx, T_BRACKET_LROUND);
    st.condition.condition = *parseExpression(ctx->expCtx, ctx->s_ctx);
    check_and_get_token(ctx->s_ctx, T_BRACKET_RROUND);

    if (peek_token(ctx->s_ctx)->type == T_BRACKET_LCURLY)
        parse_block(ctx, &st.condition.caseTrue);
    else
        parse_statement(ctx, &st.condition.caseTrue);

    Ttoken* ifels = peek_token(ctx->s_ctx);
    if (ifels->type == T_KEYWORD && ifels->kw == K_ELSE) {
        get_token(ctx->s_ctx);
        if (peek_token(ctx->s_ctx)->type == T_BRACKET_LCURLY)
            parse_block(ctx, &st.condition.caseFalse);
        else
            parse_statement(ctx, &st.condition.caseFalse);
    }
    add_statement(statements, st);
}

Statement* parse_f_call(t_Expr_Parser_Init* exprCtx, Tinit* scanner, char* id) {
    Statement* st = gc_alloc(sizeof(Statement));
    st->type = expression;
    st->expression.type = function_call;
    st->expression.fCall.name = id;
    st->expression.fCall.parameters.count = 0;
    st->expression.fCall.parameters.size = 0;
    st->expression.fCall.parameters.parameters = NULL;

    check_and_get_token(scanner, T_BRACKET_LROUND);
    if (peek_token(scanner)->type != T_BRACKET_RROUND) {
        while (true) {
            Expression* ex = parseExpression(exprCtx, scanner);
            add_parameter(st->expression.fCall.parameters, *ex);
            if (check_and_peek_token(scanner, T_COMMA | T_BRACKET_RROUND)->type == T_COMMA)
                get_token(scanner);
            else
                break;
        }
    }
    check_and_get_token(scanner, T_BRACKET_RROUND);
    return st;
}

void parse_function_call(Syntax_context* ctx, Statement_collection* statements, char* id) {
    Statement* st = parse_f_call(ctx->expCtx, ctx->s_ctx, id);
    check_and_get_token(ctx->s_ctx, T_SEMICOLON);
    add_statement(statements, *st);
}

void parse_while(Syntax_context* ctx, Statement_collection* statements) {
    Statement st = {
        .type = while_loop
    };
    check_and_get_keyword(ctx->s_ctx, K_WHILE);
    check_and_get_token(ctx->s_ctx, T_BRACKET_LROUND);
    //TODO assign expression
    parseExpression(ctx->expCtx, ctx->s_ctx);
    check_and_get_token(ctx->s_ctx, T_BRACKET_RROUND);
    if (peek_token(ctx->s_ctx)->type != T_BRACKET_LCURLY)
        parse_statement(ctx, &st.while_loop.statements);
    else
        parse_block(ctx, &st.while_loop.statements);
    add_statement(statements, st);
}

void parse_return(Syntax_context* ctx, Statement_collection* statements) {
    Statement st = {
        .type = Return
    };
    check_and_get_keyword(ctx->s_ctx, K_RETURN);

    if (peek_token(ctx->s_ctx)->type == T_SEMICOLON) {
        get_token(ctx->s_ctx);
        st.ret.type = constant;
        //TODO return null
    }else {
        parseExpression(ctx->expCtx, ctx->s_ctx);
        //TODO assign expression to statement
    }
    add_statement(statements, st);
}

void parse_statement(Syntax_context* ctx, Statement_collection* statements) {
    Ttoken* tok;
    switch ((tok = check_and_peek_token(ctx->s_ctx, T_ID | T_KEYWORD | T_TYPE))->type) {
    case T_TYPE:
        parse_declaration(ctx, statements);
        break;
    case T_ID: {
        Parsed_id id = parse_id(ctx->s_ctx, ctx->current_class);
        if (check_and_peek_token(ctx->s_ctx, T_ASSIGN | T_BRACKET_LROUND)->type == T_ASSIGN)
            parse_assigmnent(ctx, statements, id);
        else
            parse_function_call(ctx, statements, id.full);
        break;
    }
    case T_KEYWORD:
        switch (check_and_peek_keyword(ctx->s_ctx, K_DO | K_FOR | K_IF | K_RETURN | K_WHILE)) {
        case K_DO:
        case K_FOR:
            fprintf(stderr, "Use of unsupported extension.");
            exit(98);
        case K_IF:
            parse_if(ctx, statements);
            break;
        case K_WHILE: 
            parse_while(ctx, statements);
            break;
        case K_RETURN: 
            parse_return(ctx, statements);
            break;
        default:
            fprintf(stderr, "Internal error on line %d in file %s.\n", __LINE__, __FILE__);
            exit(internal_error);
        }break;
    default:
        fprintf(stderr, "Internal error on line %d in file %s.\n", __LINE__, __FILE__);
        exit(internal_error);
    }
}

void parse_block(Syntax_context* ctx, Statement_collection* statements) {
    check_and_get_token(ctx->s_ctx, T_BRACKET_LCURLY);
    while (true) {
        token_type tType = peek_token(ctx->s_ctx)->type;
        if (tType == T_BRACKET_LCURLY)
            parse_block(ctx, statements);
        else if (tType == T_BRACKET_RCURLY)
            break;
        parse_statement(ctx, statements);
    }
    check_and_get_token(ctx->s_ctx, T_BRACKET_RCURLY);
}


void printStList(Statement_collection stc) {
    for(int i=0; i!=stc.count; i++) {
        Statement st = stc.statements[i];
        switch (st.type) {
            case declaration:
                printf("declaration: type %d id: %d \n", st.declaration.variable.type, st.declaration.variable.id);
            break;
            case expression:
                printf("expression\n");
            break;
            case condition:
                printf("condition\n");
            break;
            case assigment:
                printf("assignment\n");
            break;
            case while_loop:
                printf("while-loop\n");
            break;
            case Return: 
                printf("return\n");
            break;
            default: break;
        }
    }
}

void add_functionToList(Function_list* list, Function f) {
    if (list->size == list->count) {
        list->size += 10;
        list->items = gc_realloc(list->items, sizeof(Function)*list->size);
    }
    list->items[list->count++] = f;
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
        .parameters.size = 0,
        .parameters.count = 0,
        .parameters.parameters = NULL,
        .statements.size = 0,
        .statements.count = 0,
        .statements.statements = NULL,
        .statements.statements = 0 };

    Symbol_tree oldSymbols = ctx->local_symbols;
    ctx->local_symbols = symbol_tree_new();
    ctx->expCtx->local_tab = &ctx->local_symbols;

    check_and_get_token(ctx->s_ctx, T_BRACKET_LROUND);
    parse_parameters(ctx, &f.parameters);

    parse_block(ctx, &f.statements);
    f.stack_size = count_leafs(&ctx->local_symbols);
#ifdef _DEBUG
    printStList(f.statements);
#endif
    add_functionToList(&ctx->functions, f);

    ctx->local_symbols = oldSymbols;
    ctx->expCtx->local_tab = &ctx->local_symbols;
}
