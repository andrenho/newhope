#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdbool.h>
#include <stdint.h>

typedef uint16_t BLOCK;

typedef struct Person {
	double x, y;
	double direction;
	int image;
} Person;

typedef struct Message {
	enum { MESSAGE, INPUT } type;
	int person_id;
	char* text;
	char* options[10];
} Message;

typedef union MessageResponse {
	int option;
	char* input;
} MessageResponse;

extern bool if_in_error;

//
// initialization
//
void if_init();
void if_finish();

//
// requests
//
void if_next_frame();
void if_hero_move(int speed, double direction);

//
// queries
//
void if_hero_position(double* x, double* y);
int if_people_visible(int x1, int y1, int x2, int y2, Person** people);
uint8_t if_world_tile_stack(int x, int y, BLOCK stack[10]);

//
// messages
//
Message* if_message_pending();
void if_respond_message(MessageResponse r);
int if_wrap(char* str, int columns, char*** ret);
void if_free_message(Message** msg);

#endif
