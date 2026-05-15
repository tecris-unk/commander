#ifndef PANEL_H
#define PANEL_H

#define MAX_FILES 2048
#define MAX_TABS 10

typedef struct {
    char name[256];
    int is_dir;
    int is_link;
    int selected;
    unsigned long long size;
    unsigned int mode;
    long long mtime;
} FileItem;

typedef struct {
    char path[512];
    FileItem files[MAX_FILES];
    int count;
    int cursor;
    int scroll;
} Tab;

typedef struct {
    Tab tabs[MAX_TABS];
    int tab_count;
    int active_tab;
} Panel;

void init_panel(Panel *p, const char *path);
Tab* get_active_tab(Panel *p);
void load_dir(Tab *t);
void enter_dir(Tab *t);
void up_dir(Tab *t);
void new_tab(Panel *p);
void sort_tab(Tab *t);
void close_tab(Panel *p);

#endif