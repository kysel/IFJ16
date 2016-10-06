/** Interpretr jazyka IFJ16
* @file gc.c
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#include "gc.h"
#include "return_codes.h"
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

typedef struct list_atom {
	struct list_atom* prev;
	struct list_atom* next;
} list_atom_t;

static list_atom_t *last_atom = NULL;

/**
 * \brief Deallocates all memory allocated with GC
 * \note Function is called at exit, there is no need to call it from usercode
 */
static void gc_collect_all() {
	assert(last_atom);
	list_atom_t * current_atom;
	do {
		current_atom = last_atom->prev;
		if (last_atom)
			free(last_atom);
		last_atom = current_atom;
	} while (last_atom);
}

/**
 * \brief Initializes GC
 */
void gc_init() {
	assert(last_atom == NULL);
	atexit(gc_collect_all);
	last_atom = malloc(sizeof(list_atom_t));
	if (!last_atom)
		exit(internal_error);
	last_atom->prev = NULL;
	last_atom->next = NULL;
}


/**
 * \brief Allocates memory block
 * \param size Size of the memory block, in bytes
 * \return Pointer to allocated memory block
 */
void * gc_alloc(size_t size) {
	// ReSharper disable once CppNonReclaimedResourceAcquisition
	void * ret = malloc(sizeof(list_atom_t) + size);
	if(!ret)
		exit(internal_error);

	last_atom->next = ret;
	((list_atom_t *)ret)->prev = last_atom;
	((list_atom_t *)ret)->next = NULL;
	last_atom = ret;
	return (char*)ret + sizeof(list_atom_t);
}

/**
 * \brief Deallocates memory block that was previously allocated with GC
 * \param ptr Pointer to the memory block that has to be deallocated
 */
void gc_free(void * ptr) {
	list_atom_t* atom = (list_atom_t*)(char *)ptr - sizeof(list_atom_t);
	assert(!atom);
	//todo check if atom is in list

	if (atom->prev != NULL)
		atom->prev->next = atom->next;
	if (atom->next != NULL)
		atom->next->prev = atom->prev;
	free(atom);
}
