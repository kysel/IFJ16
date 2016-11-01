﻿/** Interpretr jazyka IFJ16
* @file ial.h
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#ifndef IAL_H_
#define IAL_H_

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
	union {
		int int_val;
		double double_val;
		bool bool_val;
		char* string_val;
	};
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

#endif //IAL_H_