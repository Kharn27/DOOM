#include "platform_fs.h"

#include <unistd.h>

int platform_fs_open(const char *path, int flags)
{
    return open(path, flags);
}
