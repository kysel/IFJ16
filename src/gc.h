/** Interpretr jazyka IFJ16
* @file gc.h
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#ifndef GC_H_
#define GC_H_

#include <stddef.h>

void gc_init();
void* gc_alloc(size_t size);
void* gc_realloc(void* ptr, size_t size);
void gc_free(void* ptr);
#endif
