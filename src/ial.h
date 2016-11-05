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
<<<<<<< HEAD

#include <stdbool.h>

int find(char* s, char* search);

typedef enum {
	int_literal,
	double_literal,
	bool_literal,
	string_literal
}symbol_type_t;

typedef struct value_type {
	symbol_type_t type;
=======
#include "ast.h"



/*typedef struct Value_type_s {
	Data_type type;
>>>>>>> LA_FUTURE
	union {
		int int_val;
		double double_val;
		bool bool_val;
		char* string_val;
	};
<<<<<<< HEAD
}value_type_t;

typedef struct symbol_tree_leaf {
	const char* key;
	int id;
	/**
	 * @note value_type'll used in interpret
	 */
	//value_type_t value;
	struct symbol_tree_leaf* left;
	struct symbol_tree_leaf* right;
}symbol_tree_leaf_t;

typedef struct symbol_tree {
	symbol_tree_leaf_t* root;
	int nextId;
}symbol_tree_t;

symbol_tree_t symbol_tree_new();
symbol_tree_leaf_t* add_symbol(symbol_tree_t* root, const char* key);
symbol_tree_leaf_t* get_symbol_by_key(symbol_tree_t* root, const char* key);
=======
}Value_type;*/

typedef struct Symbol_tree_leaf_s {
	const char* key;
	int id;
	Data_type type;
	Expression* init_value;
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
>>>>>>> LA_FUTURE

#endif //IAL_H_