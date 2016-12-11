﻿/** Interpretr jazyka IFJ16
* @file main.c
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#include <stdio.h>
#include <stdlib.h>
#include "gc.h"
#include "syntax_analysis.h"
#include "interpret.h"
#include "semantic_check.h"
#include "return_codes.h"

#ifdef _MSC_VER 
#define _CRT_SECURE_NO_WARNINGS
#endif

int main(int argc, char* argv[]) {
    gc_init();

    if (argc == 1)
        return internal_error;
    FILE* fp;

    fp = fopen(argv[1], "r");

    if (fp == NULL) {
        fprintf(stderr, "Error opening file!\n");
        return internal_error;
    }

    Syntax_context* syntax = init_syntax(fp);
    parse_program(syntax);
    fclose(fp);
    check_semantic(syntax);
    execute(syntax);

    return EXIT_SUCCESS;
}
