#ifndef UI_H
#define UI_H

#include "panel.h"

void init_ui();
void shutdown_ui();
void draw_ui(Panel *left, Panel *right, int active);

int ui_prompt(const char *title, char *buffer, int bufsize);
int ui_confirm(const char *message);

#endif