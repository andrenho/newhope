#include <stdio.h>

#include "interface.h"

int main()
{
	if_load_libraries();
	if_start_game();

	if_finish();

	return 0;
}
