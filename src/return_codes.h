﻿/** Interpretr jazyka IFJ16
* @file return_codes.h
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#ifndef RETURN_CODES_H_
#define RETURN_CODES_H_

/**
 * \brief Return codes from the project's documentation
 */
typedef enum {
    lexical_analysis_error = 1,
    syntactic_analysis_error = 2,
    semantic_error_in_code = 3,
    semantic_error_in_types = 4,
    semantic_error_other = 6,
    runtime_input_error = 7,
    runtime_uninitialized_variable_access = 8,
    runtime_zero_division = 9,
    runtime_error = 10,
    internal_error = 99
} return_code;

#endif
