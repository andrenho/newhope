#include "engine/person.h"

#include <stdlib.h>

Person* person_init()
{
	Person* p = calloc(sizeof(Person), 1);
	return p;
}


void person_free(Person** p)
{
	free(*p);
	*p = NULL;
}
