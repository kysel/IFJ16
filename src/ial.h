/** Interpretr jazyka IFJ16
* @file ial.h
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#ifndef IAL_H_
#define IAL_H_
#include "ast.h"

/*typedef struct Value_type_s {
	Data_type type;
	union {
		int int_val;
		double double_val;
		bool bool_val;
		char* string_val;
	};
}Value_type;*/

typedef struct Symbol_tree_leaf_s {
	const char* key;
	int id;
	Data_type type;
	Expression* init_expr;
	/**
	 * @note Value_type_s'll used in interpret
	 */
	//Value_type value;
	struct Symbol_tree_leaf_s* left;
	struct Symbol_tree_leaf_s* right;
}Symbol_tree_leaf;

typedef struct {
	Symbol_tree_leaf* root;
	int nextId;
}Symbol_tree;

Symbol_tree symbol_tree_new();
Symbol_tree_leaf* add_symbol(Symbol_tree* root, const char* key);
Symbol_tree_leaf* get_symbol_by_key(Symbol_tree* root, const char* key);

#endif //IAL_H_