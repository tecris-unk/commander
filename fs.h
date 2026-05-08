#ifndef FS_H
#define FS_H

void copy_file(const char *src, const char *dst);
void move_file(const char *src, const char *dst);
void delete_file(const char *path);
void make_dir(const char *path);

#endif