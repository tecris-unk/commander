#ifndef UI_H
#define UI_H

#include "panel.h"

void init_ui();
void shutdown_ui();
void draw_ui(Panel *left, Panel *right, int active);

#endif