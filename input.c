#include "input.h"
#include "fs.h"
#include "ui.h"
#include <stdio.h>
#include <ncurses.h>
#include <string.h>

static void build_path(char *out, const char *dir, const char *file) {
    if (strcmp(dir, "/") == 0)
        sprintf(out, "/%s", file);
    else
        sprintf(out, "%s/%s", dir, file);
}

int handle_input(int ch, Panel *l, Panel *r, int *active) {
    Panel *p = *active == 0 ? l : r;
    Panel *other = *active == 0 ? r : l;

    Tab *t = get_active_tab(p);
    Tab *to = get_active_tab(other);

    switch (ch) {
        case 'q':
            return 1;

        case '\t':
            *active = !*active;
            break;

        case KEY_UP:
            if (t->cursor > 0) t->cursor--;
            if (t->cursor < t->scroll) t->scroll = t->cursor;
            break;

        case KEY_DOWN:
            if (t->cursor < t->count - 1) t->cursor++;
            if (t->cursor >= t->scroll + LINES - 3) t->scroll = t->cursor - (LINES - 4);
            break;

        case 10:                         // Enter
            enter_dir(t);
            break;

        case KEY_BACKSPACE:
        case 127:                        // Delete key (often same as backspace)
            up_dir(t);
            break;

        case KEY_F(5): {
            if (t->count == 0) break;
            char src[512], dst[512];
            build_path(src, t->path, t->files[t->cursor].name);
            build_path(dst, to->path, t->files[t->cursor].name);
            copy_file(src, dst);
            load_dir(to);
            break;
        }

        case KEY_F(6): {
            if (t->count == 0) break;
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
            char name[256] = "new_folder";
            if (!ui_prompt("New dir", name, sizeof(name))) break;
            build_path(path, t->path, name);
            make_dir(path);
            load_dir(t);
            break;
        }

        case KEY_F(8): {
            if (t->count == 0) break;
            if (!ui_confirm("Delete selected file? (y/n)")) break;
            char path[512];
            build_path(path, t->path, t->files[t->cursor].name);
            delete_file(path);
            load_dir(t);
            break;
        }

        case KEY_F(4): {
            if (t->count == 0) break;
            char src[512], dst[512], name[256];
            strcpy(name, t->files[t->cursor].name);
            if (!ui_prompt("Rename to", name, sizeof(name))) break;
            build_path(src, t->path, t->files[t->cursor].name);
            build_path(dst, t->path, name);
            move_file(src, dst);
            load_dir(t);
            break;
        }

<<<<<<< HEAD
        case 't': new_tab(p); break;
        case 'w': close_tab(p); break;
=======
        case 't':
            new_tab(p);
            break;

        case 'w':
            close_tab(p);
            break;
>>>>>>> 5446323 (commit)

        case KEY_LEFT:
            if (p->active_tab > 0) p->active_tab--;
            break;

        case KEY_RIGHT:
            if (p->active_tab < p->tab_count - 1) p->active_tab++;
            break;
    }

    return 0;
}