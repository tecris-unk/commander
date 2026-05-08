#include "ui.h"
#include <ncurses.h>
#include <sys/statvfs.h>

void init_ui() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
}

void shutdown_ui() {
    endwin();
}

static void draw_panel(Panel *p, int x, int w, int active) {
    Tab *t = get_active_tab(p);

    for (int i = 0; i < LINES - 3; i++) {
        int idx = t->scroll + i;
        if (idx >= t->count) break;

        if (idx == t->cursor && active)
            attron(A_REVERSE);

        mvprintw(i, x, "%-*s", w - 1, t->files[idx].name);

        if (idx == t->cursor && active)
            attroff(A_REVERSE);
    }
}

static void draw_status(Tab *t) {
    struct statvfs s;
    statvfs(t->path, &s);
    long free = (s.f_bavail * s.f_frsize) / 1024 / 1024;

    mvprintw(LINES - 2, 0, "Path: %s | Files: %d | Free: %ld MB", t->path, t->count, free);
}

void draw_ui(Panel *l, Panel *r, int active) {
    clear();

    draw_panel(l, 0, COLS / 2, active == 0);
    draw_panel(r, COLS / 2, COLS / 2, active == 1);

    Panel *p = active == 0 ? l : r;
    draw_status(get_active_tab(p));

    mvprintw(LINES - 1, 0,
             "TAB-switch | Enter-open | Backspace-up | F5 copy | F6 move | F7 mkdir | F8 delete | q-exit");

    refresh();
}