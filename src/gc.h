#ifndef GC_H_
#define GC_H_

#include <stddef.h>

void gc_init();
void * gc_alloc(size_t size);
void gc_free(void * ptr);
static void gc_collect_all();
#endif
