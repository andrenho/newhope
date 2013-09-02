#ifndef UTIL_LIST_H
#define UTIL_LIST_H

typedef struct List {
	void* data;
	struct List* next;
} List;

List* list();
void add(List** ls, void* data);

// FOREACH(list, type, new_variable_name)
#define FOREACH(ls, TYPE, current) TYPE (current)=ls ? (TYPE)((ls)->data) : NULL; for(List* __ls=(ls);  (current)=(__ls ? (TYPE)__ls->data : NULL), __ls; __ls=__ls->next)

#endif
