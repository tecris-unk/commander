#include "panel.h"
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <sys/stat.h>

static void join_path(char *out, size_t out_size, const char *base, const char *name) {
    if (strcmp(base, "/") == 0) {
        snprintf(out, out_size, "/%s", name);
    } else {
        snprintf(out, out_size, "%s/%s", base, name);
    }
}

Tab* get_active_tab(Panel *p) {
    return &p->tabs[p->active_tab];
}

void load_dir(Tab *t) {
    DIR *d = opendir(t->path);
    if (!d) return;

    t->count = 0;
    struct dirent *e;

    while ((e = readdir(d)) && t->count < MAX_FILES) {
        FileItem *f = &t->files[t->count];
        strncpy(f->name, e->d_name, sizeof(f->name) - 1);
        f->name[sizeof(f->name) - 1] = 0;
        f->selected = 0;

        char full[PATH_MAX];
        join_path(full, sizeof(full), t->path, f->name);

        struct stat st;
        if (lstat(full, &st) == 0) {
            f->is_dir = S_ISDIR(st.st_mode);
            f->is_link = S_ISLNK(st.st_mode);
            f->size = (unsigned long long) st.st_size;
            f->mode = st.st_mode;
            f->mtime = (long long) st.st_mtime;
        } else {
            f->is_dir = (e->d_type == DT_DIR);
            f->is_link = (e->d_type == DT_LNK);
            f->size = 0;
            f->mode = 0;
            f->mtime = 0;
        }
        t->count++;
    }

    if (t->cursor >= t->count) t->cursor = t->count > 0 ? t->count - 1 : 0;

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
    if (t->count == 0) return;
    FileItem *f = &t->files[t->cursor];

    if (!f->is_dir) return;

    if (strcmp(f->name, "..") == 0) up_dir(t);
    else if (strcmp(f->name, ".") != 0) join_path(t->path, sizeof(t->path), t->path, f->name);

    t->cursor = 0;
    t->scroll = 0;

    load_dir(t);
}

void up_dir(Tab *t) {
    char *slash = strrchr(t->path, '/');
    if (slash && slash != t->path) *slash = 0;
    else strcpy(t->path, "/");

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