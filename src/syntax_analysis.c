/** Interpretr jazyka IFJ16
* @file syntax_analysis.c
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
#include "syntax_analysis.h"
#include "build_in.h"

void parse_class(Syntax_context* ctx);
void parse_function(Syntax_context* ctx, Data_type return_type, char* name);
void parse_block(Syntax_context* ctx, Statement_collection* statements);
void parse_statement(Syntax_context* ctx, Statement_collection* statements);
Variable parse_global_variable(Syntax_context* ctx, Data_type type);

//Add function to the funtion list
void add_functionToList(Function_list* list, Function f) {
    if (list->size == list->count) {
        list->size += 10;
        list->items = gc_realloc(list->items, sizeof(Function) * list->size);
    }
    list->items[list->count++] = f;
}

//Add buildin to the function list
void add_buildIn(Syntax_context* ctx, Function f) {
    Symbol_tree_leaf* fSym = get_symbol_by_key(&ctx->global_symbols, f.name);
    if (fSym != NULL && fSym->defined == true) {
        fprintf(stderr, "Symbol %s was previously defined.\n", f.name);
        exit(semantic_error_in_code);
    }
    fSym = add_symbol_woId(&ctx->global_symbols, f.name);
    fSym->init_expr = NULL;
    fSym->defined = true;
    add_functionToList(&ctx->functions, f);
}

//Add all buildins to the function list
void add_buildInsToCtx(Syntax_context* ctx) {
    add_symbol(&ctx->classes, "ifj16");
    add_buildIn(ctx, (Function) {.type = build_in, .name = "ifj16.readInt", .build_in = readInt});
    add_buildIn(ctx, (Function) {.type = build_in, .name = "ifj16.readDouble", .build_in = readDouble});
    add_buildIn(ctx, (Function) {.type = build_in, .name = "ifj16.readString", .build_in = readString});
    add_buildIn(ctx, (Function) { .type = build_in, .name = "ifj16.print", .build_in = print });
    add_buildIn(ctx, (Function) { .type = build_in, .name = "ifj16.length", .build_in = length });
    add_buildIn(ctx, (Function) { .type = build_in, .name = "ifj16.substr", .build_in = substr });
    add_buildIn(ctx, (Function) { .type = build_in, .name = "ifj16.compare", .build_in = compare });
    add_buildIn(ctx, (Function) { .type = build_in, .name = "ifj16.find", .build_in = findBI });
    add_buildIn(ctx, (Function) { .type = build_in, .name = "ifj16.sort", .build_in = sortBI });
}

Syntax_context* init_syntax(FILE* input_file) {
    Syntax_context* ret = gc_alloc(sizeof(Syntax_context));
    ret->s_ctx = init_scanner(input_file);
    ret->global_symbols = symbol_tree_new(false);
    ret->local_symbols = symbol_tree_new(true);
    ret->classes = symbol_tree_new(true);
    ret->functions.count = 0;
    ret->functions.size = 0;
    ret->functions.items = 0;
    ret->depth = 0;
    ret->expCtx = ExprParserInit(&ret->global_symbols, &ret->local_symbols, "");
    add_buildInsToCtx(ret);
    return ret;
}

void parse_program(Syntax_context* ctx) {
    while (peek_token(ctx->s_ctx)->type == T_KEYWORD && peek_token(ctx->s_ctx)->kw == K_CLASS)
        parse_class(ctx);
    ctx->globals = count_leafs(&ctx->global_symbols);
    check_and_get_token(ctx->s_ctx, T_EOF);
}

//Parses id (of the func or variable)
Parsed_id parse_id(Tinit* scanner, const char* currentClass) {
    Parsed_id ret = {.fullQ = false};
    Ttoken* nameTok = check_and_get_token(scanner, T_ID | T_FULL_ID);
    ret.nameTok = nameTok;
    if (nameTok->type == T_FULL_ID) {
        ret.fullQ = true;
        ret.full = nameTok->c;

        //char*(*kek)[2] = split_id(ret.full);

        char* e = strchr(ret.full, '.');
        int idx = (int)(e - ret.full);
        ret.class = gc_alloc(sizeof(char)*(idx + 1));
        strncpy(ret.class, nameTok->c, idx);
        ret.class[idx] = 0;

        int varNameLen = strlen(nameTok->c - idx - 1);
        ret.name = gc_alloc(sizeof(char)*(varNameLen+1));
        strncpy(ret.name, nameTok->c + idx + 1, varNameLen);
        ret.name[varNameLen] = 0;
    }
    else {
        ret.class = (char*)currentClass;
        ret.name = nameTok->c;
        ret.full = gc_alloc(sizeof(char) * (strlen(currentClass) + strlen(nameTok->c)) + 2);
        strcpy(ret.full, currentClass);
        strcat(ret.full, ".");
        strcat(ret.full, nameTok->c);
    }
    return ret;
}

//Parses expression and checks for null returned by Expression parser
Expression* parse_expressionWrap(t_Expr_Parser_Init* symbol_tabs, Tinit* scanner) {
    Expression* ret = parseExpression(symbol_tabs, scanner);
    if (ret != NULL)
        return ret;
    fprintf(stderr, "Expected expression on line %lld\n", scanner->line);
    exit(syntactic_analysis_error);
}

//Parses entire class starting with class token
void parse_class(Syntax_context* ctx) {
    assert(peek_token(ctx->s_ctx)->type == T_KEYWORD && peek_token(ctx->s_ctx)->kw == K_CLASS);
    get_token(ctx->s_ctx); //consume class token

    Ttoken* classId = check_and_get_token(ctx->s_ctx, T_ID);

    if(get_symbol_by_key(&ctx->classes, classId->c) != NULL) {
        fprintf(stderr, "Redefinion of class '%s' on line '%lld'\n", classId->c, classId->line);
        exit(semantic_error_in_code);
    }
    add_symbol(&ctx->classes, classId->c);

    ctx->current_class = classId->c;
    check_and_get_token(ctx->s_ctx, T_BRACKET_LCURLY);
    ctx->expCtx->class_name = ctx->current_class;

    while (peek_token(ctx->s_ctx)->type != T_BRACKET_RCURLY) {
        Ttoken* nextToken = check_and_get_token(ctx->s_ctx, T_KEYWORD);
        if (nextToken->kw == K_STATIC) { //function or variable
            Ttoken* type = check_and_get_token(ctx->s_ctx, T_TYPE);
            Parsed_id name = parse_id(ctx->s_ctx, ctx->current_class);
            if (name.fullQ) {
                fprintf(stderr, "Fully qualified name not allowed here, line %lld\n", name.nameTok->line);
                exit(1337);
            }

            Ttoken* fnOrVar = check_and_peek_token(ctx->s_ctx, T_SEMICOLON | T_ASSIGN | T_BRACKET_LROUND);
            if (fnOrVar->type == T_SEMICOLON || fnOrVar->type == T_ASSIGN) {
                Variable var = parse_global_variable(ctx, type->dtype);
                Symbol_tree_leaf* varLeaf = get_symbol_by_key(&ctx->global_symbols, name.full);
                if(varLeaf != NULL && varLeaf->defined == true) {
                    fprintf(stderr, "Symbol %s was previously defined.\n", name.full);
                    exit(semantic_error_in_code);
                }
                varLeaf = add_symbol(&ctx->global_symbols, name.full);
                varLeaf->type = var.type;
                varLeaf->init_expr = var.init_expr;
                varLeaf->defined = true;
            } else if (fnOrVar->type == T_BRACKET_LROUND)
                parse_function(ctx, type->dtype, name.full);
        }
    }
    check_and_get_token(ctx->s_ctx, T_BRACKET_RCURLY);
    ctx->current_class = NULL;
    ctx->expCtx->class_name = ctx->current_class;
}

//Appends statement to the statement list, if there is no more space, list'll be firstly expanded
void add_statement(Statement_collection* collection, Statement statement) {
    if (collection->size == collection->count) {
        collection->size += 10;
        collection->statements = gc_realloc(collection->statements, sizeof(Statement) * collection->size);
    }
    collection->statements[collection->count++] = statement;
}

//Appends parameter to the parameter list, if there is no more space, list'll be firstly expanded
void add_parameter(Parameter_list* collection, Expression expr) {
    if (collection->size == collection->count) {
        collection->size += 10;
        collection->parameters = gc_realloc(collection->parameters, sizeof(Func_parameter) * collection->size);
    }
    collection->parameters[collection->count].type = 0;
    collection->parameters[collection->count].name = 0;
    collection->parameters[collection->count++].value = expr;
}

//Parses global variable definition
Variable parse_global_variable(Syntax_context* ctx, Data_type type) {
    if (type == void_t) {
        fprintf(stderr, "Invalid variable type, type \'void\' cannot be used here, l. %%\n");
        exit(semantic_error_in_types);
    }
    Variable ret = {.type = type};
    Ttoken* nextToken = check_and_get_token(ctx->s_ctx, T_ASSIGN | T_SEMICOLON);

    if (nextToken->type == T_ASSIGN) {
        ret.init_expr = parse_expressionWrap(ctx->expCtx, ctx->s_ctx);
        check_and_get_token(ctx->s_ctx, T_SEMICOLON);
    }
    else if (nextToken->type == T_SEMICOLON)
        ret.init_expr = NULL;
    return ret;
}

//Parses function parameters
void parse_parameters(Syntax_context* ctx, Parameter_list* params) {
    while (peek_token(ctx->s_ctx)->type != T_BRACKET_RROUND) {
        Ttoken* type = check_and_get_token(ctx->s_ctx, T_TYPE);
        Ttoken* name = check_and_get_token(ctx->s_ctx, T_ID);

        Func_parameter param = {
            .name = name->c,
            .type = type->dtype
        };
        params->parameters = gc_realloc(params->parameters, sizeof(Func_parameter) * (params->count + 1));
        params->parameters[params->count++] = param;

        //params' id should be ascending from zero (in local symbol table)
        Symbol_tree_leaf* symbol = add_symbol(&ctx->local_symbols, param.name);
        symbol->type = param.type;

        Ttoken* nextToken = check_and_peek_token(ctx->s_ctx, T_COMMA | T_BRACKET_RROUND);
        if (nextToken->type == T_COMMA)
            get_token(ctx->s_ctx);
    }
    check_and_get_token(ctx->s_ctx, T_BRACKET_RROUND);
}

//Searches for symbol in the symbol tables (int the local one first, then in the global)
Symbol_tree_leaf* get_symbol(Syntax_context* ctx, char* key) {
    Symbol_tree_leaf* leaf = get_symbol_by_key(&ctx->local_symbols, key);
    if (leaf == NULL)
        return get_symbol_by_key(&ctx->global_symbols, key);
    return leaf;
}

//Parses assignment to the variable
void parse_assigmnent(Syntax_context* ctx, Statement_collection* statements, Parsed_id id) {
    Symbol_tree_leaf* symbol = NULL;
    if (id.fullQ == false)
    //definitely local symbol
        symbol = get_symbol(ctx, id.name);
    if (symbol == NULL /*&& id.fullQ == true*/) {
        symbol = get_symbol(ctx, id.full);
        if (symbol == NULL)
            symbol = add_symbol(&ctx->global_symbols, id.full);
    }

    check_and_get_token(ctx->s_ctx, T_ASSIGN);
    Statement st = {
        .type = assigment,
        .assignment.target = symbol->id
    };
    Expression* sourceExpr = parse_expressionWrap(ctx->expCtx, ctx->s_ctx);
    st.assignment.source = *sourceExpr;
    add_statement(statements, st);
    check_and_get_token(ctx->s_ctx, T_SEMICOLON);
}

//Parses definition of the variable
void parse_definition(Syntax_context* ctx, Statement_collection* statements) {
    Data_type type = check_and_get_token(ctx->s_ctx, T_TYPE)->dtype;
    Parsed_id id = parse_id(ctx->s_ctx, ctx->current_class);
    if (id.fullQ) {
        fprintf(stderr, "Declaration of class variable inside method. line %d in file %s.\n", __LINE__, __FILE__);
        exit(syntactic_analysis_error);
    }

    Symbol_tree_leaf* symbol = get_symbol_by_key(&ctx->local_symbols, id.name);
    if(symbol != false && symbol->defined == true) {
        fprintf(stderr, "Symbol %s was previously defined.\n", id.name);
        exit(semantic_error_in_code);
    }
    symbol = add_symbol(&ctx->local_symbols, id.name);
    symbol->type = type;
    symbol->init_expr = NULL;
    symbol->defined = true;
    Statement st = {
        .type = declaration,
        .declaration.variable.id = symbol->id,
        .declaration.variable.init_expr = NULL,
        .declaration.variable.type = type
    };

    add_statement(statements, st);
    if (peek_token(ctx->s_ctx)->type == T_SEMICOLON)
        get_token(ctx->s_ctx);
    else if (check_and_peek_token(ctx->s_ctx, T_ASSIGN))
        parse_assigmnent(ctx, statements, id);
}

//Parses the if statement and it's compound statements
void parse_if(Syntax_context* ctx, Statement_collection* statements) {
    Statement st = {
        .type = condition,
        .condition.caseTrue.size = 0,
        .condition.caseFalse.size = 0
    };

    check_and_get_keyword(ctx->s_ctx, K_IF);
    check_and_get_token(ctx->s_ctx, T_BRACKET_LROUND);
    st.condition.condition = *parse_expressionWrap(ctx->expCtx, ctx->s_ctx);
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

//Parses function call
Expression* parse_f_call(t_Expr_Parser_Init* exprCtx, Tinit* scanner, char* id) {
    Expression* st = gc_alloc(sizeof(Statement));
    st->type = function_call;
    st->fCall.name = id;
    st->fCall.parameters.count = 0;
    st->fCall.parameters.size = 0;
    st->fCall.parameters.parameters = NULL;

    check_and_get_token(scanner, T_BRACKET_LROUND);
    if (peek_token(scanner)->type != T_BRACKET_RROUND) {
        while (true) {
            Expression* ex = parse_expressionWrap(exprCtx, scanner);
            add_parameter(&st->fCall.parameters, *ex);
            if (check_and_peek_token(scanner, T_COMMA | T_BRACKET_RROUND)->type == T_COMMA)
                get_token(scanner);
            else
                break;
        }
    }
    check_and_get_token(scanner, T_BRACKET_RROUND);
    return st;
}

//Parses function call and adds fCall statement to the statement list
void parse_function_call(Syntax_context* ctx, Statement_collection* statements, char* id) {
    Statement* st = gc_alloc(sizeof(Statement));
    st->type = expression;
    st->expression = *parse_f_call(ctx->expCtx, ctx->s_ctx, id);
    check_and_get_token(ctx->s_ctx, T_SEMICOLON);
    add_statement(statements, *st);
}

//Parses the while statement and adds it to the statement list
void parse_while(Syntax_context* ctx, Statement_collection* statements) {
    Statement st = {
        .type = while_loop
    };
    check_and_get_keyword(ctx->s_ctx, K_WHILE);
    check_and_get_token(ctx->s_ctx, T_BRACKET_LROUND);
    st.while_loop.condition = *parse_expressionWrap(ctx->expCtx, ctx->s_ctx);
    check_and_get_token(ctx->s_ctx, T_BRACKET_RROUND);
    if (peek_token(ctx->s_ctx)->type != T_BRACKET_LCURLY)
        parse_statement(ctx, &st.while_loop.statements);
    else
        parse_block(ctx, &st.while_loop.statements);
    add_statement(statements, st);
}

//Parses return statement and adds it to the statement list
void parse_return(Syntax_context* ctx, Statement_collection* statements) {
    Statement st = {
        .type = Return
    };
    check_and_get_keyword(ctx->s_ctx, K_RETURN);

    if (peek_token(ctx->s_ctx)->type == T_SEMICOLON) {
        st.ret.type = constant;
        st.ret.constant.type = void_t;
    } else
        st.ret = *(Return_statement*)parse_expressionWrap(ctx->expCtx, ctx->s_ctx);
    get_token(ctx->s_ctx); // gets ';', cause parseExpression leaves it
    add_statement(statements, st);
}

//Parses all statements
void parse_statement(Syntax_context* ctx, Statement_collection* statements) {
    Ttoken* tok;
    switch ((tok = check_and_peek_token(ctx->s_ctx, T_ID | T_FULL_ID | T_KEYWORD | T_TYPE))->type) {
        case T_TYPE:
            if (ctx->depth <= 1)
                parse_definition(ctx, statements);
            else {
                fprintf(stderr, "Local variable cannot be defined inside a compound statements line %lld.\n", tok->line);
                exit(syntactic_analysis_error);
            }
            break;
        case T_FULL_ID:
        case T_ID: {
            Parsed_id id = parse_id(ctx->s_ctx, ctx->current_class);
            if (check_and_peek_token(ctx->s_ctx, T_ASSIGN | T_BRACKET_LROUND)->type == T_ASSIGN)
                parse_assigmnent(ctx, statements, id);
            else
                parse_function_call(ctx, statements, id.full);
            break;
        }
        case T_KEYWORD:
            switch (check_and_peek_keyword(ctx->s_ctx, K_IF | K_RETURN | K_WHILE)) {
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
            }
            break;
        default:
            fprintf(stderr, "Internal error on line %d in file %s.\n", __LINE__, __FILE__);
            exit(internal_error);
    }
}

//Parses coupound statement from '{' to '}'
void parse_block(Syntax_context* ctx, Statement_collection* statements) {
    ctx->depth++;
    check_and_get_token(ctx->s_ctx, T_BRACKET_LCURLY);
    while (true) {
        token_type tType = peek_token(ctx->s_ctx)->type;
        if (tType == T_BRACKET_LCURLY)
            parse_block(ctx, statements);
        if (peek_token(ctx->s_ctx)->type == T_BRACKET_RCURLY)
            break;
        parse_statement(ctx, statements);
    }
    check_and_get_token(ctx->s_ctx, T_BRACKET_RCURLY);
    ctx->depth--;
}

//Prints simplified statement collection
void printStList(Statement_collection stc) {
    for (int i = 0; i != stc.count; i++) {
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

//Parses whole function and adds it to the function list
void parse_function(Syntax_context* ctx, Data_type return_type, char* name) {
    Function f = {
        .name = name,
        .return_type = return_type,
        .type = user,
        .parameters.size = 0,
        .parameters.count = 0,
        .parameters.parameters = NULL,
        .statements.size = 0,
        .statements.count = 0,
        .statements.statements = NULL};

    Symbol_tree_leaf* fSym = get_symbol_by_key(&ctx->global_symbols, name);
    if (fSym != NULL &&fSym->defined == true) {
        fprintf(stderr, "Symbol %s was previously defined.\n", name);
        exit(semantic_error_in_code);
    }
    fSym = add_symbol_woId(&ctx->global_symbols, name);
    fSym->init_expr = NULL;
    fSym->defined = true;

    //Setup context for function
    Symbol_tree oldSymbols = ctx->local_symbols;
    ctx->local_symbols = symbol_tree_new(true);
    ctx->expCtx->local_tab = &ctx->local_symbols;
    ctx->expCtx->inside_func = true;

    check_and_get_token(ctx->s_ctx, T_BRACKET_LROUND);
    parse_parameters(ctx, &f.parameters);

    parse_block(ctx, &f.statements);
    f.stack_size = count_leafs(&ctx->local_symbols);
    f.local_symbols = ctx->local_symbols;
#ifdef DEBUG
    printStList(f.statements);
#endif
    if (f.return_type == void_t)
        add_statement(&f.statements, (Statement) { .type = Return, .ret.type = constant, .ret.constant.type = void_t });
    add_functionToList(&ctx->functions, f);


    ctx->expCtx->inside_func = false;
    ctx->local_symbols = oldSymbols;
    ctx->expCtx->local_tab = &ctx->local_symbols;
}
