#include "ui.h"
#include <ncurses.h>
#include <sys/statvfs.h>
#include <string.h>

void init_ui() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    tart_color();
    use_default_colors();
    init_pair(1, COLOR_CYAN, -1);
    init_pair(2, COLOR_MAGENTA, -1);
}

void shutdown_ui() {
    endwin();
}

static void draw_panel(Panel *p, int x, int w, int active) {
    Tab *t = get_active_tab(p);

    for (int i = 0; i < LINES - 3; i++) {
        int idx = t->scroll + i;
        if (idx >= t->count) break;

        if (t->files[idx].is_dir) attron(COLOR_PAIR(1));
        else if (t->files[idx].is_link) attron(COLOR_PAIR(2));
        if (idx == t->cursor && active) attron(A_REVERSE);

        mvprintw(i, x, "%-*s", w - 1, t->files[idx].name);

        if (idx == t->cursor && active) attroff(A_REVERSE);
        if (t->files[idx].is_dir) attroff(COLOR_PAIR(1));
        else if (t->files[idx].is_link) attroff(COLOR_PAIR(2));
    }
}

static void draw_status(Tab *t) {
    struct statvfs s;
    statvfs(t->path, &s);
    long free = (s.f_bavail * s.f_frsize) / 1024 / 1024;

    "TAB switch | <-/-> tabs | t/w tabs | Enter/open | Backspace/up | F4 ren F5 cp F6 mv F7 mk F8 rm | q");
}

void draw_ui(Panel *l, Panel *r, int active) {
    clear();

    draw_panel(l, 0, COLS / 2, active == 0);
    draw_panel(r, COLS / 2, COLS / 2, active == 1);

    Panel *p = active == 0 ? l : r;
    draw_status(get_active_tab(p));

    mvprintw(LINES - 1, 0,
             "TAB-switch panel | <-/-> tabs | t/w tab+/- | Enter-open | Backspace-up | F5/F6/F7/F8 | q-exit");

    refresh();
}

int ui_confirm(const char *question) {
    mvprintw(LINES - 1, 0, "%-*s", COLS - 1, "");
    mvprintw(LINES - 1, 0, "%s", question);
    refresh();
    int ch;
    do { ch = getch(); } while (ch != 'y' && ch != 'Y' && ch != 'n' && ch != 'N');
    return ch == 'y' || ch == 'Y';
}

int ui_prompt(const char *title, char *out, int out_size) {
    if (out_size <= 1) return 0;
    echo();
    curs_set(1);
    mvprintw(LINES - 1, 0, "%-*s", COLS - 1, "");
    mvprintw(LINES - 1, 0, "%s: ", title);
    getnstr(out, out_size - 1);
    noecho();
    curs_set(0);
    if (strlen(out) == 0) return 0;
    return 1;
}

void ui_message(const char *msg) {
    mvprintw(LINES - 1, 0, "%-*s", COLS - 1, "");
    mvprintw(LINES - 1, 0, "%s", msg);
    refresh();
}