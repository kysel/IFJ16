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
#include <stdbool.h>
#include "ast.h"

int find(char* s, char* search);
char* sort(char* s);

typedef enum {
    int_literal,
    double_literal,
    bool_literal,
    string_literal
} symbol_type_t;

Symbol_tree symbol_tree_new(bool inc);


/**
 * \brief Counts the number of the leafs in a tree
 * \param tree Symbol tree 
 * \return Number ob the leaves
 */
int count_leafs(Symbol_tree* tree);

/**
* \brief Add leaf with given key. If leaf with provided
* key already exist it's returned too.
* \param root Symbol tree where to search for the leaf
* \param key Leaf key
* \return Pointer to the inserted leaf
*/
Symbol_tree_leaf* add_symbol(Symbol_tree* root, const char* key);


/**
* \brief Add leaf with given key but does NOT increase id.
* If leaf with provided key already exist it's returned too.
* \param tree Symbol tree where to search for the leaf
* \param key Leaf key
* \return Pointer to the inserted leaf
*/
Symbol_tree_leaf* add_symbol_woId(Symbol_tree* tree, const char* key);

/**
 * \brief Search for key with given key
 * \param root Symbol tree where to search for.
 * \param key Leaf key
 * \return If leaf with provided key is found then pointer to that leaf,
 * oherwise null
 */
Symbol_tree_leaf* get_symbol_by_key(Symbol_tree* root, const char* key);

/**
* \brief Search for key with given id
* \param tree Symbol tree where to search for.
* \param id Leaf id
* \return If leaf with provided id is found then pointer to that leaf,
* oherwise null
*/
Symbol_tree_leaf* get_symbol_by_id(Symbol_tree* tree, const int id);

#endif //IAL_H_
