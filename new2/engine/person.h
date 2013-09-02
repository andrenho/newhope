#ifndef ENGINE_PERSON_H
#define ENGINE_PERSON_H

typedef enum Speed { STOPPED, WALKING, RUNNING } Speed;

typedef struct Person {
	double x, y;
	int direction;
	Speed speed;
} Person;

struct World;

Person* person_init(double x, double y);
void person_free(Person** p);

void person_step(Person* p, struct World* w);
void person_start_running(Person* p, int direction);
void person_stop_running(Person* p);

#endif
