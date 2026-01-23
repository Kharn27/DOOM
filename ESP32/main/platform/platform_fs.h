#ifndef PLATFORM_FS_H
#define PLATFORM_FS_H

#include <fcntl.h>

int platform_fs_open(const char *path, int flags);

#endif
