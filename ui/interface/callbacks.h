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

typedef struct {
	MessageResponse (*message)(Message*);
	void (*lua_error)(const char*);
} CALLBACK;

void if_install_callbacks(MessageResponse (*message_cb)(Message*),
		void (*lua_error_cb)(const char*));

#endif
