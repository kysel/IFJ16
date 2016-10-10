/** Interpretr jazyka IFJ16
* @file ial.c
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#include "ial.h"
#include "return_codes.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>


int find(char* s, char* search) {
// https://en.wikipedia.org/wiki/Knuth%E2%80%93Morris%E2%80%93Pratt_algorithm
// http://www.ms.mff.cuni.cz/~kopecky/vyuka/dis/02/dis02_v3.html
}

symbol_tree_leaf_t* add_to_symbol_tree(symbol_tree_leaf_t* root, const char* key) {
	assert(root);
	int balance = strcmp(root->key, key);
	if (balance == 0)
		return root;

	symbol_tree_leaf_t** newLeaf = NULL;
	if (balance > 0) {
		if (root->left != NULL)
			return add_to_symbol_tree(root->left, key);
		*newLeaf = root->left;
	}
	if (balance < 0) {
		if (root->right != NULL)
			return add_to_symbol_tree(root->right, key);
		*newLeaf = root->left;
	}

	if (!newLeaf) {
		*newLeaf = malloc(sizeof(symbol_tree_leaf_t));
		if (!*newLeaf)
			exit(internal_error);
		(*newLeaf)->left = NULL;
		(*newLeaf)->right = NULL;
		(*newLeaf)->key = key;
		return *newLeaf;
	}
	//this should NOT be reached!
	assert(false);
	return NULL;
}


