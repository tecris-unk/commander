#include "input.h"
#include "fs.h"
#include <stdio.h>
#include <ncurses.h>
#include <string.h>

static void build_path(char *out, const char *dir, const char *file) {
    if (strcmp(dir, "/") == 0) sprintf(out, "/%s", file);
    else sprintf(out, "%s/%s", dir, file);
}

int handle_input(int ch, Panel *l, Panel *r, int *active) {
    Panel *p = *active == 0 ? l : r;
    Panel *other = *active == 0 ? r : l;

    Tab *t = get_active_tab(p);
    Tab *to = get_active_tab(other);

    switch (ch) {
        case 'q': return 1;
        case '\t': *active = !*active; break;

            if (t->cursor > 0) t->cursor--;
            if (t->cursor < t->scroll) t->scroll = t->cursor;
            break;
        case KEY_DOWN:
            if (t->cursor < t->count - 1) t->cursor++;
            if (t->cursor >= t->scroll + LINES - 3) t->scroll = t->cursor - (LINES - 4);
            break;

        case 10: enter_dir(t); break;
        case KEY_BACKSPACE:
        case 127:
            up_dir(t);
            break;

        case KEY_F(5): {
            char src[512], dst[512];
            build_path(src, t->path, t->files[t->cursor].name);
            build_path(dst, to->path, t->files[t->cursor].name);
            copy_file(src, dst);
            load_dir(to);
            break;
        }

        case KEY_F(6): {
            char src[512], dst[512];
            build_path(src, t->path, t->files[t->cursor].name);
            build_path(dst, to->path, t->files[t->cursor].name);
            move_file(src, dst);
            load_dir(t);
            load_dir(to);
            break;
        }

        case KEY_F(7): {
            char path[512];
            build_path(path, t->path, "new_folder");
            make_dir(path);
            load_dir(t);
            break;
        }

        case KEY_F(8): {
            char path[512];
            build_path(path, t->path, t->files[t->cursor].name);
            delete_file(path);
            load_dir(t);
            break;
        }

        case 't': new_tab(p); break;
        case 'w': close_tab(p); break;

        case KEY_LEFT:
            if (p->active_tab > 0) p->active_tab--;
            break;
        case KEY_RIGHT:
            if (p->active_tab < p->tab_count - 1) p->active_tab++;
            break;
    }

    return 0;
}