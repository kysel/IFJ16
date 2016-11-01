/** Interpretr jazyka IFJ16
* @file syntaxAnalysis.c
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#include "syntaxAnalysis.h"

void parse_program(Syntax_context* ctx) {
	//todo: Inicializace scanneru

	/**
	 * while token != endOfFile; parse_class(ctx);
	 */
}

void parse_class(Syntax_context* ctx){
	/**
	 * vytvoření kontextu pro třídu
	 * 
	 * 
	 * tady se bude volat parsování
	 * a) statické (globální) proměnné
	 * b) funkce
	 * 
	 * 
	 * ukončení kontextu třídy
	 */
}

void parse_function(Syntax_context* ctx) {
	/**
	 * Vytvoření kontextu funkce
	 * 
	 * parsování parametrů této funkce
	 * 
	 * parsování exprešnů (těla funkce)
	 * 
	 * ukončení kontextu funkce
	 */
}