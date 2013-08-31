#ifndef UI_UI_H
#define UI_UI_H

#include <stdbool.h>

typedef struct UI {
} UI;

UI* ui_init();
void ui_free(UI** u);

bool ui_active();
void ui_do_events();
void ui_render();

#endif
