
#include "ui.h"
#include "input.h"
#include "panel.h"
#include <signal.h>

void handle_resize(int sig) {
    endwin();
    refresh();
    clear();
}

int main() {
    signal(SIGWINCH, handle_resize);

    init_ui();

    Panel left, right;
    init_panel(&left, ".");
    init_panel(&right, ".");

    int active = 0;

    while (1) {
        draw_ui(&left, &right, active);
        int ch = getch();
        if (handle_input(ch, &left, &right, &active)) break;
    }

    shutdown_ui();
    return 0;
}


// =======================
// BUILD
// gcc main.c ui.c panel.c fs.c input.c -lncurses -o fm
// =======================
