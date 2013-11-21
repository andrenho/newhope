#ifndef CALLBACKS_H
#define CALLBACKS_H

typedef struct Message {
	enum { MESSAGE=1, INPUT } type;
	int person_id;
	int image;
	const char* text;
	const char* options[10];
} Message;

typedef union MessageResponse {
	int option;
	char* input;
} MessageResponse;

void if_install_callbacks(MessageResponse (*callback)(Message*));

#endif
