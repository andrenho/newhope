#include "interface.h"

#include <stdint.h>
#include <stdio.h>

int main()
{
	// initialize interface
	if_init();

	// get tile stack
	uint16_t stack[10];
	int n = if_world_tile_stack(0, 0, stack);
	for(int i=0; i<n; i++) {
		printf("%d\n", stack[i]);
	}

	// finish interface
	if_finish();
}
