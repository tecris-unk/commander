#include "ui.h"
#include "input.h"
#include "panel.h"
#include <signal.h>
#include <stdlib.h>
<<<<<<< HEAD
=======
#include <ncurses.h>
#include <unistd.h>     // для getcwd
#include <limits.h>     // для PATH_MAX (обычно)
#include <string.h>     // для strcpy, strerror

// На случай, если PATH_MAX не определён (например, в некоторых системах)
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif
>>>>>>> 5446323 (commit)

void handle_resize(int sig) {
    (void)sig;
    endwin();
    refresh();
    clear();
}

int main() {
    // Временно отключим обработку SIGWINCH для стабильной работы клавиш
    // signal(SIGWINCH, handle_resize);

    init_ui();

<<<<<<< HEAD
=======
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        // Если не удалось получить текущую директорию – используем "."
        strcpy(cwd, ".");
    }

>>>>>>> 5446323 (commit)
    Panel *left = calloc(1, sizeof(Panel));
    Panel *right = calloc(1, sizeof(Panel));
    if (!left || !right) {
        shutdown_ui();
        free(left);
        free(right);
        return 1;
    }
<<<<<<< HEAD
    init_panel(left, ".");
    init_panel(right, ".");
=======
    init_panel(left, cwd);
    init_panel(right, cwd);
>>>>>>> 5446323 (commit)

    int active = 0;

    while (1) {
        draw_ui(left, right, active);
        int ch = getch();
        if (handle_input(ch, left, right, &active)) break;
    }

    shutdown_ui();
    free(left);
    free(right);
    return 0;
}