/** Interpretr jazyka IFJ16
* @file gc.c
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#include "gc.h"
#include "return_codes.h"
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

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

static void remove_from_alloc_list(list_atom_t* atom) {
	if (atom == last_atom)
		last_atom = atom->prev;
	if (atom->prev != NULL)
		atom->prev->next = atom->next;
	if (atom->next != NULL)
		atom->next->prev = atom->prev;
}

static bool alloc_list_contains(list_atom_t* atom) {
	list_atom_t * current_atom = last_atom;
	do {
		if (current_atom == atom)
			return true;
		current_atom = current_atom->prev;
	} while (current_atom);
	return false;
}

static void gc_free_internal(list_atom_t* ptr) {
	assert(alloc_list_contains(ptr));
	remove_from_alloc_list(ptr);
	free(ptr);
}

/**
 * \brief Initializes GC
 */
void gc_init() {
#ifndef GC_BYPASS
	assert(last_atom == NULL);
	atexit(gc_collect_all);
	last_atom = malloc(sizeof(list_atom_t));
	if (!last_atom)
		exit(internal_error);
	last_atom->prev = NULL;
	last_atom->next = NULL;
#endif
}

/**
* \brief Reallocates memory block
* \param ptr pointer to old memory block
* \param size Size of the memory block, in bytes
* \return Pointer to allocated memory block
*/
void *gc_realloc(void* ptr, size_t size) {
#ifndef GC_BYPASS
	list_atom_t* atom = ptr == NULL ? NULL : (list_atom_t*)((char *)ptr - sizeof(list_atom_t));

	if (atom)
		remove_from_alloc_list(atom);

	// ReSharper disable once CppNonReclaimedResourceAcquisition
	void *ret = realloc(atom, sizeof(list_atom_t) + size);
	if(ret == NULL) {
		fprintf(stderr, "Unable to allocate memory, exiting\n");
		exit(internal_error);
	}

	last_atom->next = ret;
	((list_atom_t *)ret)->prev = last_atom;
	((list_atom_t *)ret)->next = NULL;
	last_atom = ret;
	return (char*)ret + sizeof(list_atom_t);
#else
	void *ret = realloc(ptr, size);
	if (ret == NULL) {
		fprintf(stderr, "Unable to reallocate memory, exiting\n");
		exit(internal_error);
	}
	return ret;
#endif
}

/**
 * \brief Allocates memory block
 * \param size Size of the memory block, in bytes
 * \return Pointer to allocated memory block
 */
void * gc_alloc(size_t size) {
#ifndef GC_BYPASS
	return gc_realloc(NULL, size);
#else
	void *ret = malloc(size);
	if (ret == NULL) {
		fprintf(stderr, "Unable to allocate memory, exiting\n");
		exit(internal_error);
	}
	return ret;
#endif
}

/**
 * \brief Deallocates memory block that was previously allocated with GC
 * \param ptr Pointer to the memory block that has to be deallocated
 */
void gc_free(void * ptr) {
#ifndef GC_BYPASS
	list_atom_t* atom = (list_atom_t*)((char *)ptr - sizeof(list_atom_t));
	gc_free_internal(atom);
#else
	free(ptr);
#endif
}
