﻿/** Interpretr jazyka IFJ16
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

//int find(char* s, char* search) {
// https://en.wikipedia.org/wiki/Knuth%E2%80%93Morris%E2%80%93Pratt_algorithm
// http://www.ms.mff.cuni.cz/~kopecky/vyuka/dis/02/dis02_v3.html
//}

symbol_tree_t symbol_tree_new() {
	return (symbol_tree_t) { .root = NULL, .nextId = 0 };
}

symbol_tree_leaf_t* add_symbol_impl(symbol_tree_t* tree, symbol_tree_leaf_t* leaf, const char* key, int depth) {
	symbol_tree_leaf_t** newLeaf = NULL;
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
		*newLeaf = gc_alloc(sizeof(symbol_tree_leaf_t));
		(*newLeaf)->left = NULL;
		(*newLeaf)->right = NULL;
		(*newLeaf)->key = key;
		(*newLeaf)->id = tree->nextId++;
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
symbol_tree_leaf_t* add_symbol(symbol_tree_t* tree, const char* key) {
	assert(tree);
	return add_symbol_impl(tree, tree->root, key, 0);
}


symbol_tree_leaf_t* get_symbol_by_key_impl(symbol_tree_leaf_t* node, const char* key) {
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
symbol_tree_leaf_t* get_symbol_by_key(symbol_tree_t* tree, const char* key) {
	assert(tree);
	return get_symbol_by_key_impl(tree->root, key);
}


/*
 *Currently not available, in future it should be faster than searching by key
 *symbol_tree_leaf_t* get_symbol_by_id(const symbol_tree_leaf_t* root, const int id) {
	assert(root);
	assert(root->id != 0);
	assert(id != 0);
	
}*/