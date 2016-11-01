/** Interpretr jazyka IFJ16
* @file binaryTree-test.c
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#include "gc.h"
#include "ial.h"
#include <stdio.h>

int main()
{
	gc_init();
	
	Symbol_tree st = symbol_tree_new();
	if(st.root == NULL)
		printf("[OK] init table\n");
	else
		printf("[FAIL] init table\n");

	Symbol_tree* tree = &st;
	if (add_symbol(tree, "test")!= NULL)
		printf("[OK] Add first symbol\n");
	else
		printf("[FAIL] Add first symbol\n");

	Symbol_tree_leaf* test2 = add_symbol(tree, "test2");
	if (test2 == add_symbol(tree, "test2"))
		printf("[OK] Add symbol\n");
	else 
		printf("[FAIL] Add symbol\n");

	if(test2 == get_symbol_by_key(tree, "test2"))
		printf("[OK] Get symbol\n");
	else
		printf("[FAIL] Get symbol\n");
	return 0;
}
