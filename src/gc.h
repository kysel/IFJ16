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

/**
 * \brief Initializes garbage collection and add handler to atexit
 */
void gc_init();


/**
 * \brief Allocates block of memory
 * \param size Size of the allocated memory block
 * \return Successfully allocated memory block
 */
void* gc_alloc(size_t size);

/**
 * \brief Changes the size of the memory block
 * \param ptr Pointer to already allocated memory block
 * \param size New size of the memory block
 * \return Pointer to reallocated memory block
 */
void* gc_realloc(void* ptr, size_t size);

/**
 * \brief Free up memory previvously allocated by gc_alloc
 * \param ptr Pointer to the memory block that has to be freed
 */
void gc_free(void* ptr);
#endif
