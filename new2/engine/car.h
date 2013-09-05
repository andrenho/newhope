#ifndef ENGINE_CAR_H
#define ENGINE_CAR_H

typedef struct Car {
	int x, y;
} Car;

Car* car_init(int x, int y);
void car_free(Car** c);

#endif

