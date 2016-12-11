/** Interpretr jazyka IFJ16
* @file semantic_check.c
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#include "semantic_check.h"
#include "interpret.h"
#include "gc.h"
#include "return_codes.h"
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

//Creates new error
Error new_err(const int ret_code, const char* format, ...) {
    va_list args;
    va_start(args, format);
    size_t len = vsnprintf(NULL, 0, format, args);
    char* buf = gc_alloc(sizeof(char)*(len + 2));
    vsnprintf(buf, len + 3, format, args);
    strcat(buf, "\n");
    va_end(args);
    return (Error) { .description = buf, .ret_code = ret_code };
}

//Appends error to the end of the Err_list
void append_err(Err_list* list, const Error err) {
    if (list->size == list->count) {
        list->size += 10;
        list->err = gc_realloc(list->err, sizeof(Error) * list->size);
    }
    list->err[list->count++] = err;
}

//Prints all errors
void print_errs(const Err_list errs) {
    for (int i = 0; i != errs.count; i++)
        fprintf(stderr, errs.err[i].description);
}

//Gets first not zero error code from error list
int get_exit_code(const Err_list errs) {
    for (int i = 0; i != errs.count; i++)
        if (errs.err[i].ret_code != 0)
            return errs.err[i].ret_code;
    return 0;
}

//Splits full id into two strings containing class and name
char*(*split_id(char* fullId))[2]{
    char*(*ret)[2] = gc_alloc(sizeof(char*) * 2);

    char* e = strchr(fullId, '.');
    int idx = (int)(e - fullId);
    (*ret)[0] = gc_alloc(sizeof(char)*(idx + 1));
    strncpy((*ret)[0], fullId, idx);
    (*ret)[0][idx] = 0;

    int varNameLen = strlen(fullId - idx - 1);
    (*ret)[1] = gc_alloc(sizeof(char)*(varNameLen + 1));
    strncpy((*ret)[1], fullId + idx + 1, varNameLen);
    (*ret)[1][varNameLen] = 0;
    return ret;
}

//Checks for presence of the Main.run function
void check_main(Sem_ctx* ctx) {
    Function* main = getFunc(ctx->s_ctx, "Main.run");
    if (main == NULL) {
        append_err(&ctx->errs, new_err(semantic_error_in_code, "Function Main.run does not exist."));
        return;
    }
    if(main->return_type != void_t)
        append_err(&ctx->errs, new_err(semantic_error_in_code, "Function Main.run does not return void."));
    if (main->parameters.count != 0)
        append_err(&ctx->errs, new_err(98, "Function 'Main.run' have invalid number of arguments."));
}

//Checks whether the function comply with the semantic rules
void check_func(Sem_ctx* ctx, const Function f) {
    bool hasReturned = false;
    char* className = (*split_id((char*)f.name))[0];
    for (int i = 0; i <= f.local_symbols.nextId; i++) {
        Symbol_tree_leaf* symbol = get_symbol_by_id((Symbol_tree*)&f.local_symbols, i);
        if (symbol == NULL)
            continue;
        char* fqName = gc_alloc(sizeof(char)*(strlen(className) + strlen(symbol->key) + 2));
        strcpy(fqName, className);
        strcat(fqName, ".");
        strcat(fqName, symbol->key);
        Symbol_tree_leaf* glSymbol = get_symbol_by_key((Symbol_tree*)&ctx->s_ctx->global_symbols, fqName);
        if (glSymbol == NULL || glSymbol->id < 0) {
            gc_free(fqName);
            continue;
        }
        fprintf(stderr, "Variable and function name collision detected '%s'\n", fqName);
        gc_free(fqName);
        exit(semantic_error_in_code);
    }
}

void check_semantic(Syntax_context* ctx) {
    Sem_ctx ctxVal = (Sem_ctx) { .s_ctx = ctx, .errs.count = 0, .errs.size = 0, .errs.err = NULL };
    Sem_ctx* sem = &ctxVal;
    check_main(sem);

    for (int i = 0; i != sem->s_ctx->functions.count; i++) {
        Function f = sem->s_ctx->functions.items[i];
        if (f.type == build_in)
            continue;
        check_func(sem, f);
    }

    if (sem->errs.count != 0) {
        print_errs(sem->errs);
        exit(get_exit_code(sem->errs));
    }
}
