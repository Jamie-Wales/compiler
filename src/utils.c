#include "utils.h"
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_types/_s_ifmt.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int test_file(const char* file)
{
    struct stat st;
    if (lstat(file, &st) != 0) {
        fprintf(stderr, "lstat failure\n");
        perror("lstat");
        return -1;
    }

    if (S_IFREG != (st.st_mode & S_IFMT)) {
        char* error = "";
        switch (st.st_mode & S_IFMT) {
        case S_IFSOCK:
            error = "socket";
            break;
        case S_IFLNK:
            error = "sym link";
            break;
        case S_IFBLK:
            error = "block device";
            break;
        case S_IFDIR:
            error = "directory";
            break;
        case S_IFCHR:
            error = "character device";
            break;
        case S_IFIFO:
            error = "FIFO";
            break;
        }
        errorf("Error on file %s, filetype is %s \n", file, error);
        return -1;
    }

    return 0;
}

void errorf(const char* format, ...)
{
    int errnum = errno;
    va_list ap;
    va_start(ap, format);
    fprintf(stderr, "ERROR: ");
    vfprintf(stderr, format, ap);

    if (errnum != 0) {
        fprintf(stderr, ": %s", strerror(errnum));
    }
    fprintf(stderr, "\n");
    va_end(ap);
    exit(EXIT_FAILURE);
}
