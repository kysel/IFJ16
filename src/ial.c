/** Interpretr jazyka IFJ16
* @file ial.c
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ial.h"
#include "gc.h"

int find(char* s, char* search) {
    int position = -1; //defaultne vracena hodnota vyskytu

    if (!search) //hledame-li prazdny string, vracim defaultní hodnotu
        return position;
    if (strcmp(search, "") == 0) //hledame-li prazdny string, vracim 0
        return 0;

    int lenSearch = strlen(search); //zjistujeme delku hledaneho retezce
    int lenString = strlen(s); //zjistujeme delku textoveho retezce

    int* p = gc_alloc(sizeof(int) * lenString); //alokace pole

    p[0] = -1;

    int k = 1;
    int l;
    int m;

    while (k < lenString) {
        l = k - 1;
        m = l;
        do {
            m = p[m];
        } while ((m == 0 || s[m] == s[l]));
        p[k] = m + 1;
        k++;
    }

    int i = 0; //indexy jsou od nuly
    int j = 0;

    while ((i < lenString) && (j < lenSearch)) {
        while ((j >= 0) && (search[j] != s[i])) {
            j = p[j];
        }
        i++;
        j++;
    }
    //printf("hledany delka = %d  i = %d, j = %d \n", lenSearch, i, j); //debug info
    if (j <= lenSearch)
        position = i - j;
    else
        position = 0;


    return position;
}

/**
 * \brief Sort string value using ShellSort alghoritm.
 * \param s Input string
 * \return Ordinally sorted string
 */
char* sort(char* s) {
    int n = strlen(s) - 1; 
    int step = n / 2; // prvni krok polovina poctu prvku
    int i; // iterator od prvniho k poslednimu prvku
    int j; // prochazeni paralelnich n-tic
    char h; // k prohozeni

    while (step > 0) {
        for (i = 0; i < n; i++) { // cykly pro generovani paralelnich n-tic
            j = i - step + 1;      
            while (j >= 0 && s[j] > s[j + step]) { // prochazeni paralelnich n-tic
                h = s[j];
                s[j] = s[j + step];
                s[j + step] = h; // prohozeni dvojic na indexech rozdilnych o krok
                j = j - step; // snizeni indexu o krok
            }
        }
        step = step / 2; // redukce kroku dvema
    }

    return s;
}

Symbol_tree symbol_tree_new(bool incIds) {
    if (incIds)
        return (Symbol_tree) {.root = NULL, .nextId = 0, .inc = true};
    return (Symbol_tree) {.root = NULL, .nextId = -1, .inc = false};
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

    if (leaf == NULL) //root should be added
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
        (*newLeaf)->id = tree->inc ? tree->nextId++ : tree->nextId--;
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

void remove_symbol(Symbol_tree* tree, const char* key) {
    Symbol_tree_leaf* node = tree->root;
    Symbol_tree_leaf** ptrFromPrnt = &tree->root;
    while(true) {
        int balance = strcmp(node->key, key);
        if (balance == 0) {
            *ptrFromPrnt = NULL;
            return;
        }
        if(balance < 0) {
            ptrFromPrnt = &node->left;
            node = node->left;
        } else if(balance > 0) {
            ptrFromPrnt = &node->right;
            node = node->right;
        }
        if (node == NULL)
            return;
    }
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
