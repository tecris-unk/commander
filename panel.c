#include "panel.h"
#include <dirent.h>
#include <string.h>
#include <unistd.h>

Tab* get_active_tab(Panel *p) {
    return &p->tabs[p->active_tab];
}

void load_dir(Tab *t) {
    DIR *d = opendir(t->path);
    if (!d) return;

    t->count = 0;
    struct dirent *e;

    while ((e = readdir(d)) && t->count < MAX_FILES) {
        strcpy(t->files[t->count].name, e->d_name);
        t->files[t->count].is_dir = (e->d_type == DT_DIR);
        t->files[t->count].selected = 0;
        t->count++;
    }

    closedir(d);
}

void init_panel(Panel *p, const char *path) {
    p->tab_count = 1;
    p->active_tab = 0;

    strcpy(p->tabs[0].path, path);
    p->tabs[0].cursor = 0;
    p->tabs[0].scroll = 0;

    load_dir(&p->tabs[0]);
}

void enter_dir(Tab *t) {
    FileItem *f = &t->files[t->cursor];

    if (!f->is_dir) return;

    if (strcmp(f->name, "..") == 0) {
        char *slash = strrchr(t->path, '/');
        if (slash && slash != t->path) *slash = 0;
        else strcpy(t->path, "/");
    } else if (strcmp(f->name, ".") != 0) {
        strcat(t->path, "/");
        strcat(t->path, f->name);
    }

    t->cursor = 0;
    t->scroll = 0;

    load_dir(t);
}

void new_tab(Panel *p) {
    if (p->tab_count >= MAX_TABS) return;
    p->tabs[p->tab_count] = *get_active_tab(p);
    p->active_tab = p->tab_count++;
}

void close_tab(Panel *p) {
    if (p->tab_count == 1) return;

    for (int i = p->active_tab; i < p->tab_count - 1; i++)
        p->tabs[i] = p->tabs[i + 1];

    p->tab_count--;
    if (p->active_tab >= p->tab_count)
        p->active_tab = p->tab_count - 1;
}