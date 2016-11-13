/** Interpretr jazyka IFJ16
* @file ial.c
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#include "ial.h"
#include "gc.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

//int find(char* s, char* search) {
// https://en.wikipedia.org/wiki/Knuth%E2%80%93Morris%E2%80%93Pratt_algorithm
// http://www.ms.mff.cuni.cz/~kopecky/vyuka/dis/02/dis02_v3.html
//}

Symbol_tree symbol_tree_new() {
	return (Symbol_tree) { .root = NULL, .nextId = 0 };
}

int count_leafs_impl(Symbol_tree_leaf* leaf) {
    if (leaf == NULL)
        return 0;
    int ret = 1;
    if (leaf->left != NULL)
        ret += count_leafs_impl(leaf->left);
    if (leaf->right != NULL)
        ret += count_leafs_impl(leaf->right);
    return ret;
}

int count_leafs(Symbol_tree* tree) {
    return count_leafs_impl(tree->root);
}

Symbol_tree_leaf* add_symbol_impl(Symbol_tree* tree, Symbol_tree_leaf* leaf, const char* key, int depth) {
	Symbol_tree_leaf** newLeaf = NULL;
	//add root
	if (leaf == NULL)
		newLeaf = &leaf;
	else {
		int balance = strcmp(leaf->key, key);
		if (balance == 0)
			return leaf;

		if (balance < 0) {
			if (leaf->left != NULL)
				return add_symbol_impl(tree, leaf->left, key, ++depth);
			newLeaf = &leaf->left;
		} else {
			if (leaf->right != NULL)
				return add_symbol_impl(tree, leaf->right, key, ++depth);
			newLeaf = &leaf->right;
		}
	}

	if (!*newLeaf) {
		*newLeaf = gc_alloc(sizeof(Symbol_tree_leaf));
		(*newLeaf)->left = NULL;
		(*newLeaf)->right = NULL;
		(*newLeaf)->key = key;
		(*newLeaf)->id = tree->nextId++;
		(*newLeaf)->type = 0;
		if (tree->root == NULL)
			tree->root = *(newLeaf);
		return *newLeaf;
	}
	//this should NOT be reached!
	assert(false);
	return NULL;
}

/**
 * \brief Add key to symbol tree after initialization the leaf is returned.
 * If leaf with the provided key already exist it's returned too.
 * \param tree Root leaf from where to start the search
 * \param key Leaf key
 * \return Pointer to inserted leaf
 */
Symbol_tree_leaf* add_symbol(Symbol_tree* tree, const char* key) {
	assert(tree);
	return add_symbol_impl(tree, tree->root, key, 0);
}


Symbol_tree_leaf* get_symbol_by_key_impl(Symbol_tree_leaf* node, const char* key) {
	assert(node);

	int balance = strcmp(node->key, key);
	if (balance == 0)
		return node;

	if (balance < 0 && node->left != NULL)
		return get_symbol_by_key_impl(node->left, key);

	if (balance > 0 && node->right != NULL)
		return get_symbol_by_key_impl(node->right, key);
	return NULL;
}

/**
 * \brief Retrieve leaf from symbol tree
 * \param tree Root leaf from where to start the search
 * \param key Leaf key
 * \return If the leaf with the provided key exist, then it's returned, otherwise NULL
 */
Symbol_tree_leaf* get_symbol_by_key(Symbol_tree* tree, const char* key) {
	assert(tree);
    if (tree->root != NULL)
        return get_symbol_by_key_impl(tree->root, key);
    return NULL;
}


/*
 *Currently not available, in future it should be faster than searching by key
 *Symbol_tree_leaf* get_symbol_by_id(const Symbol_tree_leaf* root, const int id) {
	assert(root);
	assert(root->id != 0);
	assert(id != 0);
	
}*/