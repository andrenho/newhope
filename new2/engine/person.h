#ifndef ENGINE_PERSON_H
#define ENGINE_PERSON_H

typedef struct Person {
} Person;

Person* person_init();
void person_free(Person** p);

#endif
