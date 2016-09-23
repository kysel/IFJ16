#include "gc.h"

int main()
{
	gc_init();
	for (int i=0; i!=50; i++) {
		gc_alloc(50);
	}
	return 0;	
}
