/** Interpretr jazyka IFJ16
* @file interpret.c
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#include <string.h>
#include "syntaxAnalysis.h"
#include "ast.h"
#include "interpret.h"
#include "gc.h"

Function* getFunc(Inter_ctx* ctx, char* fkName) {
    for (int i=0; i!= ctx->s->functions.count; i++) {
        if (strcmp(ctx->s->functions.items[i].name, fkName))
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

Value parseExpr(Inter_ctx* ctx, Expression* ex) {
    return (Value) { .init = true };
}

Value executeStatement(Inter_ctx* ctx, Statement* st) {
    switch (st->type) {
        case declaration:
        {
            ctx->loc_stack->val[st->declaration.variable.id] = parseExpr(ctx, st->declaration.variable.init_expr);
        }
        break;
        case expression: break;
        case condition: break;
        case assigment: break;
        case while_loop: break;
        case Return: break;
        default: break;
    }
}

Value executeFunc(Inter_ctx* ctx, Function* f) {
    Value ret = {
        .type = f->return_type,
        .init = true
    };
    Value_list* old_stack = ctx->loc_stack;
    ctx->loc_stack = alloc_stack(f->stack_size);

    for (int i=0; i!= f->statements.count; i++) {
        executeStatement(ctx, &f->statements.statements[i]);
    }

    ctx->loc_stack = old_stack;
    return ret;
}

void execute(Syntax_context * syntax) {
    Inter_ctx ctxNoPtr = { .s = syntax };
    Inter_ctx* ctx = &ctxNoPtr;

    executeFunc(ctx, getFunc(ctx, "Main.run"));
}