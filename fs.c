#include "fs.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

void copy_file(const char *src, const char *dst) {
    FILE *s = fopen(src, "rb");
    FILE *d = fopen(dst, "wb");
    if (!s || !d) return;

    char buf[4096];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), s)))
        fwrite(buf, 1, n, d);

    fclose(s);
    fclose(d);
}

void move_file(const char *src, const char *dst) {
    rename(src, dst);
}

void delete_file(const char *path) {
    remove(path);
}

void make_dir(const char *path) {
    mkdir(path, 0755);
}