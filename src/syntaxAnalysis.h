/** Interpretr jazyka IFJ16
* @file syntaxAnalysis.h
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#ifndef SYNTAXANALYSIS_H_
#define SYNTAXANALYSIS_H_

#include "ial.h"

typedef struct {
	//todo: přidat sem scanner resp. jeho kontext

	Symbol_tree* global_symbols, class_symbols, local_symbols;
}Syntax_context;

#endif
