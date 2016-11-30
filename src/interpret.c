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
#include "syntax_analysis.h"
#include "ast.h"
#include "interpret.h"
#include "gc.h"
#include "return_codes.h"

//#define SEM_CHECK

Value eval_expr(Inter_ctx* ctx, Expression* ex);
Return_value eval_statement(Inter_ctx* ctx, Statement* st);
Return_value eval_func(Inter_ctx* ctx, FunctionCall* fCall);
Return_value eval_st_list(Inter_ctx* ctx, const Statement_collection* statements);

Function* getFunc(Syntax_context* ctx, char* fkName) {
    for (int i = 0; i != ctx->functions.count; i++) {
        if (strcmp(ctx->functions.items[i].name, fkName) == 0)
            return &ctx->functions.items[i];
    }
    return NULL;
}

Value_list* alloc_stack(int size) {
    Value_list* ret = gc_alloc(sizeof(Value_list));
    ret->val = gc_alloc(sizeof(Value) * size);
    ret->count = size;
    ret->size = size;
    for (int i = 0; i != ret->count; i++)
        ret->val[i].init = false;
    return ret;
}

Data_type resulting_type(Data_type t1, Data_type t2) {
    if (t1 == t2)
        return t1;
    if ((t1 == double_t && t2 == int_t) || (t1 == int_t && t2 == double_t))
        return double_t;
    return t1;
}

Value implicit_cast(Value val, Data_type to) {
    if (val.type == to)
        return val;
    Value ret = {.type = to,.init = true};
    switch (to) {
        case bool_t:
            if (val.type == int_t)
                ret.b = val.i != 0;
            else if (val.type == double_t)
                ret.b = val.d != 0;
            else
                break;
            return ret;
        case double_t:
            if (val.type == int_t) {
                ret.d = (double)val.i;
                return ret;
            }
            break;
        case string_t: {
            int strLength;
            if (val.type == int_t) {
                strLength = snprintf(NULL, 0, "%d", val.i);
                ret.s = gc_alloc(sizeof(char) * (strLength + 1));
                snprintf(ret.s, strLength + 1, "%d", val.i);
            }
            else if (val.type == double_t) {
#ifdef JAVA_SUCK
                if (((long)val.d - val.d) == 0)
                    strLength = snprintf(NULL, 0, "%.1f", val.d);
                else
                    strLength = snprintf(NULL, 0, "%g", val.d);
                ret.s = gc_alloc(sizeof(char) * (strLength + 1));
                if (((long)val.d - val.d) == 0)
                    snprintf(ret.s, strLength + 1, "%.1f", val.d);
                else
                    snprintf(ret.s, strLength + 1, "%g", val.d);
#else
                strLength = snprintf(NULL, 0, "%g", val.d);
                ret.s = gc_alloc(sizeof(char) * (strLength + 1));
                snprintf(ret.s, strLength + 1, "%g", val.d);
#endif
            }
            else
                break;
            return ret;
        }
        default: break;
    }

    fprintf(stderr, "Invalid cast from %d to %d. line %d in file %s.\n", val.type, to, __LINE__, __FILE__);
    exit(semantic_error_in_types);
}

void set_val(Inter_ctx* ctx, int id, Value val) {
    if (val.init == false) {
        fprintf(stderr, "Use of uninitialized variable\n");
        exit(runtime_uninitialized_variable_access);
    }
    Value* ret;
    if (id >= 0)
        ret = &ctx->loc_stack->val[id];
    else
        ret = &ctx->globals->val[-(id + 1)];
    *ret = val;
}

Value* get_val(Inter_ctx* ctx, int id) {
    Value* ret;
    if (id >= 0)
        ret = &ctx->loc_stack->val[id];
    else
        ret = &ctx->globals->val[-(id + 1)];
    if(ret->init == false) {
        fprintf(stderr, "Use of uninitialized variable\n");
        exit(runtime_uninitialized_variable_access);
    }
    return ret;
}

Return_value eval_func(Inter_ctx* ctx, FunctionCall* fCall) {
    Function* f = getFunc(ctx->s, fCall->name);
#ifdef SEM_CHECK
    if (f == NULL) {
        fprintf(stderr, "Function %s does not exist. line %d in file %s.\n", fCall->name, __LINE__, __FILE__);
        exit(semantic_error_in_code);
    }
    if (f->type == user && f->parameters.count != fCall->parameters.count) {
        fprintf(stderr, "Function %s was called with invalid params count. line %d in file %s.\n", fCall->name, __LINE__, __FILE__);
        exit(semantic_error_in_types);
    }
#endif

    Value_list* oldStack = ctx->loc_stack;
    Value_list* newStack;
    if (f->type == user)
        newStack = alloc_stack(f->stack_size);
    else if (f->type == build_in)
        newStack = alloc_stack(fCall->parameters.count);
    else {
        fprintf(stderr, "Ouch this type of function i don't know. line %d in file %s.\n", __LINE__, __FILE__);
        exit(semantic_error_in_code);
    }
    for (int i = 0; i != fCall->parameters.count; i++)
        newStack->val[i] = eval_expr(ctx, &fCall->parameters.parameters[i].value);
    ctx->loc_stack = newStack;

    Return_value ret = {.returned = false};
    if (f->type == user)
        ret = eval_st_list(ctx, &f->statements);
    else if (f->type == build_in)
        ret = (Return_value) {.val = f->build_in(*ctx->loc_stack), .returned = true};

#ifdef SEM_CHECK
    if (ret.returned != true) {
        fprintf(stderr, "Error during execution of the '%s' method body. line %d in file %s.\n", fCall->name, __LINE__, __FILE__);
        exit(syntactic_analysis_error);
    }
#endif
    if (f->return_type == string_t && ret.val.type != string_t) {
        fprintf(stderr, "Invalid return in '%s'. line %d in file %s.\n", fCall->name, __LINE__, __FILE__);
        exit(syntactic_analysis_error);
    }

    ctx->loc_stack = oldStack;
    if (f->type == user)
        return (Return_value) { .val = implicit_cast(ret.val, f->return_type), .returned = true };
    return (Return_value) { .val = ret.val, .returned = true };
}

Value eval_op_tree(Inter_ctx* ctx, BinOpTree* tree) {
    Value left = eval_expr(ctx, tree->left_expr);
    Value right = eval_expr(ctx, tree->right_expr);

    if(left.init == false || right.init == false) {
        fprintf(stderr, "Use uf uninitialized variable.\n");
        exit(runtime_uninitialized_variable_access);
    }

    Value ret = {.init = true, .type = void_t};
    switch (tree->BinOp) {
        case OP_ADD:
            if (left.type == string_t || right.type == string_t) {
                Value lStr = implicit_cast(left, string_t);
                Value rStr = implicit_cast(right, string_t);
                ret.type = string_t;
                ret.s = gc_alloc(sizeof(char) * (strlen(lStr.s) + strlen(rStr.s) + 1));
                ret.s[0] = 0;
                strcat(ret.s, lStr.s);
                strcat(ret.s, rStr.s);
                break;
            }
            if (resulting_type(left.type, right.type) == int_t) {
                Value l = implicit_cast(left, int_t);
                Value r = implicit_cast(right, int_t);
                ret.type = int_t;
                ret.i = l.i + r.i;
                break;
            }
            if (resulting_type(left.type, right.type) == double_t) {
                Value l = implicit_cast(left, double_t);
                Value r = implicit_cast(right, double_t);
                ret.type = double_t;
                ret.d = l.d + r.d;
            }
            break;
        case OP_SUB:
            if (resulting_type(left.type, right.type) == int_t) {
                Value l = implicit_cast(left, int_t);
                Value r = implicit_cast(right, int_t);
                ret.type = int_t;
                ret.i = l.i - r.i;
                break;
            }
            if (resulting_type(left.type, right.type) == double_t) {
                Value l = implicit_cast(left, double_t);
                Value r = implicit_cast(right, double_t);
                ret.type = double_t;
                ret.d = l.d - r.d;
            }
            break;
        case OP_MUL:
            if (resulting_type(left.type, right.type) == int_t) {
                Value l = implicit_cast(left, int_t);
                Value r = implicit_cast(right, int_t);
                ret.type = int_t;
                ret.i = l.i * r.i;
                break;
            }
            if (resulting_type(left.type, right.type) == double_t) {
                Value l = implicit_cast(left, double_t);
                Value r = implicit_cast(right, double_t);
                ret.type = double_t;
                ret.d = l.d * r.d;
            }
            break;
        case OP_DIV:
            if (resulting_type(left.type, right.type) == int_t) {
                Value l = implicit_cast(left, int_t);
                Value r = implicit_cast(right, int_t);
                ret.type = int_t;
                if(r.i == 0) {
                    fprintf(stderr, "Division by zero.\n");
                    exit(runtime_zero_division);
                }
                ret.i = l.i / r.i;
                break;
            }
            if (resulting_type(left.type, right.type) == double_t) {
                Value l = implicit_cast(left, double_t);
                Value r = implicit_cast(right, double_t);
                ret.type = double_t;
                if (r.i == 0) {
                    fprintf(stderr, "Division by zero.\n");
                    exit(runtime_zero_division);
                }
                ret.d = l.d / r.d;
            }
            break;
        case OP_LOWER:
            if (resulting_type(left.type, right.type) == int_t) {
                Value l = implicit_cast(left, int_t);
                Value r = implicit_cast(right, int_t);
                ret.type = bool_t;
                ret.b = l.i < r.i;
                break;
            }
            if (resulting_type(left.type, right.type) == double_t) {
                Value l = implicit_cast(left, double_t);
                Value r = implicit_cast(right, double_t);
                ret.type = bool_t;
                ret.b = l.d < r.d;
            }
            break;
        case OP_GREATER:
            if (resulting_type(left.type, right.type) == int_t) {
                Value l = implicit_cast(left, int_t);
                Value r = implicit_cast(right, int_t);
                ret.type = bool_t;
                ret.b = l.i > r.i;
                break;
            }
            if (resulting_type(left.type, right.type) == double_t) {
                Value l = implicit_cast(left, double_t);
                Value r = implicit_cast(right, double_t);
                ret.type = bool_t;
                ret.b = l.d > r.d;
            }
            break;
        case OP_LOWER_EQUAL:
            if (resulting_type(left.type, right.type) == int_t) {
                Value l = implicit_cast(left, int_t);
                Value r = implicit_cast(right, int_t);
                ret.type = bool_t;
                ret.b = l.i <= r.i;
                break;
            }
            if (resulting_type(left.type, right.type) == double_t) {
                Value l = implicit_cast(left, double_t);
                Value r = implicit_cast(right, double_t);
                ret.type = bool_t;
                ret.b = l.d <= r.d;
            }
            break;
        case OP_GREATER_EQUAL:
            if (resulting_type(left.type, right.type) == int_t) {
                Value l = implicit_cast(left, int_t);
                Value r = implicit_cast(right, int_t);
                ret.type = bool_t;
                ret.b = l.i >= r.i;
                break;
            }
            if (resulting_type(left.type, right.type) == double_t) {
                Value l = implicit_cast(left, double_t);
                Value r = implicit_cast(right, double_t);
                ret.type = bool_t;
                ret.b = l.d >= r.d;
            }
            break;
        case OP_BOOL_EQUAL:
            if (resulting_type(left.type, right.type) == int_t) {
                Value l = implicit_cast(left, int_t);
                Value r = implicit_cast(right, int_t);
                ret.type = bool_t;
                ret.b = l.i == r.i;
                break;
            }
            if (resulting_type(left.type, right.type) == double_t) {
                Value l = implicit_cast(left, double_t);
                Value r = implicit_cast(right, double_t);
                ret.type = bool_t;
                ret.b = l.d == r.d;
            }
            break;
        case OP_NOT_EQUAL:
            if (resulting_type(left.type, right.type) == int_t) {
                Value l = implicit_cast(left, int_t);
                Value r = implicit_cast(right, int_t);
                ret.type = bool_t;
                ret.b = l.i != r.i;
                break;
            }
            if (resulting_type(left.type, right.type) == double_t) {
                Value l = implicit_cast(left, double_t);
                Value r = implicit_cast(right, double_t);
                ret.type = bool_t;
                ret.b = l.d != r.d;
            }
            break;
        default: break;
    }

    if (ret.type == void_t) {
        fprintf(stderr, "lolternal error. line %d in file %s.\n", __LINE__, __FILE__);
        exit(internal_error);
    }
    return ret;
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
                    return (Value) {.type = void_t, .init = true};
                case int_t:
                    return (Value) {.type = int_t, .i = ex->constant.li, .init = true};
                case double_t:
                    return (Value) {.type = double_t, .d = ex->constant.d, .init = true};
                case bool_t:
                    fprintf(stderr, "Boolean type is not supported yet. line %d in file %s.\n", __LINE__, __FILE__);
                    exit(1337);
                case string_t:
                    return (Value) {.type = string_t, .s = ex->constant.c, .init = true};
                default:
                    fprintf(stderr, "Fuckuplo se to u expressionu. line %d in file %s.\n", __LINE__, __FILE__);
                    exit(internal_error);
            }
        }
        case bin_op_tree:
            return eval_op_tree(ctx, &ex->tree);
        default:
            fprintf(stderr, "Fuckuplo se to u expressionu. line %d in file %s.\n", __LINE__, __FILE__);
            exit(internal_error);
    }
}

void eval_declaration(Inter_ctx* ctx, Statement* st) {
    if (st->declaration.variable.init_expr != NULL)
        set_val(ctx, st->declaration.variable.id, eval_expr(ctx, st->declaration.variable.init_expr));
}

Return_value eval_cond(Inter_ctx* ctx, If_statement* ifSt) {
    if (eval_expr(ctx, &ifSt->condition).b)
        return eval_st_list(ctx, &ifSt->caseTrue);
    return  eval_st_list(ctx, &ifSt->caseFalse);
}

Return_value eval_while(Inter_ctx* ctx, While_statement* stWhile) {
    Return_value ret;
    while (eval_expr(ctx, &stWhile->condition).b)
        if ((ret = eval_st_list(ctx, &stWhile->statements)).returned)
            return ret;
    return (Return_value) { .returned = false };
}

Return_value eval_statement(Inter_ctx* ctx, Statement* st) {
    switch (st->type) {
    case declaration: eval_declaration(ctx, st);
        break;
    case expression:
        return (Return_value) { .returned = false, .val = eval_expr(ctx, &st->expression) };
    case condition: return eval_cond(ctx, &st->condition);
    case assigment:
        set_val(ctx, st->assignment.target, eval_expr(ctx, &st->assignment.source));
        break;
    case while_loop: return eval_while(ctx, &st->while_loop);
    case Return:
        return (Return_value) { .returned = true, .val = eval_expr(ctx, &st->ret) };
    default: break;
    }
    return (Return_value) { .returned = false };
}

Return_value eval_st_list(Inter_ctx* ctx, const Statement_collection* statements) {
    Return_value ret;
    for (int i = 0; i != statements->count; i++)
        if ((ret = eval_statement(ctx, &statements->statements[i])).returned)
            return ret;
    return (Return_value) {.returned = false, .val.type = void_t};
}

void init_globals_impl(Inter_ctx* ctx, Symbol_tree_leaf* leaf) {
    if(leaf == NULL)
        return;
    if (leaf->init_expr != NULL)
        set_val(ctx, leaf->id, eval_expr(ctx, leaf->init_expr));
    if (leaf->left != NULL)
        init_globals_impl(ctx, leaf->left);
    if (leaf->right != NULL)
        init_globals_impl(ctx, leaf->right);
}

void init_globals(Inter_ctx* ctx, Symbol_tree* tree) {
    for (int i=0; i!= ctx->globals->count; i++)
        ctx->globals->val[i].init = false;
    init_globals_impl(ctx, tree->root);
}

void execute(Syntax_context* syntax) {
    Inter_ctx ctxNoPtr = {.s = syntax};
    Inter_ctx* ctx = &ctxNoPtr;

#ifdef SEM_CHECK
    if (getFunc(ctx, "Main.run")->parameters.count != 0) {
        fprintf(stderr, "Invalid number of parameters in function Main.run. line %d in file %s.\n", __LINE__, __FILE__);
        exit(1337);
    }
#endif

    ctx->globals = alloc_stack(ctx->s->globals);
    init_globals(ctx, &ctx->s->global_symbols);
    FunctionCall runMain = {.name = "Main.run",.parameters.count = 0};
    eval_func(ctx, &runMain);
}
