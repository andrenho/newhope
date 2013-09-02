#include "util/list.h"

#include <stdlib.h>

List* list()
{
	return NULL;
}


void add(List** ls, void* data)
{
	List* ln = calloc(sizeof(List), 1);
	ln->data = data;
	ln->next = NULL;

	if(*ls == NULL) {
		*ls = ln;
	} else {
		List* lx = *ls;
		while(lx->next) {
			lx = lx->next;
		}
		lx->next = ln;
	}
}
