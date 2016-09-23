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

void gc_init() {
	assert(last_atom == NULL);
	atexit(gc_collect_all);
	last_atom = malloc(sizeof(list_atom_t));
	if (!last_atom)
		exit(internal_error);
	last_atom->prev = NULL;
	last_atom->next = NULL;
}

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

void gc_free(void * ptr) {
	list_atom_t* atom = (list_atom_t*)(char *)ptr - sizeof(list_atom_t);
	assert(!atom);
	//todo check if atom is in list
	atom->prev->next = atom->next;
	free(atom);
}

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
