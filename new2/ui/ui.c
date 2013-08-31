#include "ui/ui.h"

#include <stdlib.h>

UI* ui_init()
{
	UI* u = malloc(sizeof(UI));
	return u;
}


void ui_free(UI** u)
{
	free(*u);
	*u = NULL;
}


bool ui_active()
{
	return true;
}


void ui_do_events()
{
}


void ui_render()
{
}
