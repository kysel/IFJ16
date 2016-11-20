/** Interpretr jazyka IFJ16
* @file interpret.c
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#include <string.h>
#include <stdlib.h>
#include "syntaxAnalysis.h"
#include "ast.h"
#include "interpret.h"
#include "gc.h"

#define SEM_CHECK

//TODO: replace exit codes 1337 with better ones

Value eval_expr(Inter_ctx* ctx, Expression* ex);
Return_value eval_statement(Inter_ctx* ctx, Statement* st);
Return_value eval_func(Inter_ctx* ctx, FunctionCall* fCall);
Return_value eval_st_list(Inter_ctx* ctx, const Statement_collection* statements);

Function* getFunc(Inter_ctx* ctx, char* fkName) {
    for (int i=0; i!= ctx->s->functions.count; i++) {
        if (strcmp(ctx->s->functions.items[i].name, fkName) == 0)
            return &ctx->s->functions.items[i];
    }
    return NULL;
}

Value_list* alloc_stack(int size) {
    Value_list* ret = gc_alloc(sizeof(Value_list));
    ret->val = gc_alloc(sizeof(Value)*size);
    ret->count = size;
    ret->size = size;
    for (int i = 0; i != ret->count; i++)
        ret->val[i].init = false;
    return ret;
}

Data_type resulting_type(Data_type t1, Data_type t2) {
    if (t1 == t2)
        return t1;
    if (t1 == double_t && t2 == int_t || t1 == int_t && t2 == double_t)
        return double_t;
    return t1;
}

Value implicit_cast(Value val, Data_type to) {
    if (val.type == to)
        return val;
    Value ret = { .type = to,.init = true };
    switch (to) {
    case bool_t:
        if (val.type == int_t)
            ret.b = val.i != 0;
        else if (val.type == double_t)
            ret.b = val.d != 0;
        else
            break;
        return ret;
    case int_t:
        if (val.type == double_t) {
            ret.i = (int)val.d;
            return  ret;
        }
        break;
    case double_t:
        if (val.type == int_t) {
            ret.d = (double)val.i;
            return ret;
        }
        break;
    case string_t:
    {
        int strLength;
        if (val.type == int_t) {
            strLength = snprintf(NULL, 0, "%d", val.i);
            ret.s = gc_alloc(sizeof(char)*(strLength + 1));
            snprintf(ret.s, strLength, "%d", val.i);
        }
        else if (val.type == double_t) {
            strLength = snprintf(NULL, 0, "%g", val.d);
            ret.s = gc_alloc(sizeof(char)*(strLength + 1));
            snprintf(ret.s, strLength, "%g", val.d);
        }
        else
            break;
    }
    default: break;
    }

    fprintf(stderr, "Invalid cast from %d to %d. line %d in file %s.\n", val.type, to, __LINE__, __FILE__);
    exit(1337);
}

Value* get_val(Inter_ctx *ctx, int id) {
    if (id >= 0)
        return &ctx->loc_stack->val[id];
    return &ctx->globals->val[-(id + 1)];
}

Return_value eval_func(Inter_ctx* ctx, FunctionCall* fCall) {
    Function* f = getFunc(ctx, fCall->name);
#ifdef SEM_CHECK
    if (f == NULL) {
        fprintf(stderr, "Function %s does not exist. line %d in file %s.\n", fCall->name, __LINE__, __FILE__);
        exit(1337);
    }
    if (f->type == user && f->parameters.count != fCall->parameters.count) {
        fprintf(stderr, "Function %s was called with invalid params count. line %d in file %s.\n", fCall->name, __LINE__, __FILE__);
        exit(1337);
    }
#endif

    Value_list* oldStack = ctx->loc_stack;
    ctx->loc_stack = alloc_stack(f->stack_size);
    for (int i = 0; i != fCall->parameters.count; i++)
        *get_val(ctx, i) = eval_expr(ctx, &fCall->parameters.parameters[i].value);

    Return_value ret = { .returned = false };
    if (f->type == user)
        ret = eval_st_list(ctx, &f->statements);
    else if (f->type == build_in)
        ret = (Return_value) { .val = f->build_in(*ctx->loc_stack), .returned = true };

#ifdef SEM_CHECK
    if(ret.returned != true) {
        fprintf(stderr, "Error during execution of the '%s' method body. line %d in file %s.\n", fCall->name, __LINE__, __FILE__);
        exit(1337);
    }
    if(f->return_type == string_t && ret.val.type!= string_t) {
        fprintf(stderr, "Invalid return in '%s'. line %d in file %s.\n", fCall->name, __LINE__, __FILE__);
        exit(1337);
    }
#endif

    ctx->loc_stack = oldStack;

    return (Return_value) { .val = implicit_cast(ret.val, f->return_type), .returned = true };
}

Value eval_expr(Inter_ctx* ctx, Expression* ex) {
    //TODO!
    switch (ex->type) {
        case function_call:
            return eval_func(ctx, &ex->fCall).val;
        case variable: return *get_val(ctx, ex->variable);
        case constant: {
            switch (ex->constant.type) {
                case void_t:
                    return(Value) { .type = void_t, .init = true };
                case int_t:
                    return(Value) { .type = int_t, .i = ex->constant.li, .init = true };
                case double_t:
                    return(Value) { .type = double_t, .d = ex->constant.d, .init = true };
                case bool_t:
                    fprintf(stderr, "Boolean type is not supported yet. line %d in file %s.\n", __LINE__, __FILE__);
                    exit(1337);
                case string_t:
                    return(Value) { .type = string_t, .s = ex->constant.c, .init = true };
                default: break;
            }
        }
        break;
        case bin_op_tree: break;
        default: break;
    }
    return (Value) { .init = true };
}

void eval_declaration(Inter_ctx* ctx, Statement* st) {
    if (st->declaration.variable.init_expr != NULL)
        *get_val(ctx, st->declaration.variable.id) = eval_expr(ctx, st->declaration.variable.init_expr);
}

void eval_assign(Inter_ctx* ctx, Statement* st) {
    *get_val(ctx, st->assignment.target) = eval_expr(ctx, &st->assignment.source);
}

void eval_cond(Inter_ctx* ctx, If_statement* ifSt) {
    if (eval_expr(ctx, &ifSt->condition).b)
        eval_st_list(ctx, &ifSt->caseTrue);
    else
        eval_st_list(ctx, &ifSt->caseFalse);
}

void eval_while(Inter_ctx* ctx, While_statement* stWhile) {
    while (eval_expr(ctx, &stWhile->condition).b)
        eval_st_list(ctx, &stWhile->statements);
}

Return_value eval_statement(Inter_ctx* ctx, Statement* st) {
    switch (st->type) {
    case declaration:   eval_declaration(ctx, st); break;
    case expression:
        return (Return_value) { .returned = false, .val = eval_expr(ctx, &st->expression) };
    case condition:     eval_cond(ctx, &st->condition); break;
    case assigment:     eval_assign(ctx, st); break;
    case while_loop:    eval_while(ctx, &st->while_loop); break;
    case Return: 
        return (Return_value) { .returned = true, .val = eval_expr(ctx, &st->expression) };
    default: break;
    }
    return (Return_value) { .returned = false };
}

Return_value eval_st_list(Inter_ctx* ctx, const Statement_collection* statements) {
    Return_value ret;
    for (int i = 0; i != statements->count; i++)
        if ((ret = eval_statement(ctx, &statements->statements[i])).returned)
            return ret;
    return (Return_value) { .returned = true, .val.type = void_t };
}

/*Value executeFunc(Inter_ctx* ctx, Function* f) {
    Value ret = {
        .type = f->return_type,
        .init = true
    };
    Value_list* old_stack = ctx->loc_stack;
    ctx->loc_stack = alloc_stack(f->stack_size);

    eval_st_list(ctx, &f->statements);

    ctx->loc_stack = old_stack;
    return ret;
}*/

void execute(Syntax_context * syntax) {
    Inter_ctx ctxNoPtr = { .s = syntax };
    Inter_ctx* ctx = &ctxNoPtr;

#ifdef SEM_CHECK
    if(getFunc(ctx, "Main.run")->parameters.count != 0) {
        fprintf(stderr, "Invalid number of parameters in function Main.run. line %d in file %s.\n", __LINE__, __FILE__);
        exit(1337);
    }
#endif

    ctx->globals = alloc_stack(ctx->s->globals);
    FunctionCall runMain = { .name = "Main.run",.parameters.count = 0 };
    eval_func(ctx, &runMain);
}