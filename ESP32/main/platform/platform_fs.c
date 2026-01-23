#include "platform_fs.h"

#include <sys/stat.h>
#include <unistd.h>

int platform_fs_open(const char *path, int flags)
{
    return open(path, flags);
}

ssize_t platform_fs_read(int fd, void *buffer, size_t size)
{
    return read(fd, buffer, size);
}

off_t platform_fs_seek(int fd, off_t offset, int whence)
{
    return lseek(fd, offset, whence);
}

int platform_fs_size(int fd)
{
    struct stat fileinfo;

    if (fstat(fd, &fileinfo) == -1) {
        return -1;
    }

    return (int)fileinfo.st_size;
}
