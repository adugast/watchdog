#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define WATCHDOG_PATHNAME "/dev/watchdog"

#define err(format, ...) \
    fprintf(stderr, format, __VA_ARGS__)

int get_watchdog_fd()
{
    int fd = -1;

    fd = open(WATCHDOG_PATHNAME, O_RDWR);
    if (fd == -1) {
        err("Unable to open watchdog device:%s\n", WATCHDOG_PATHNAME);
        return -1;
    }

    return fd;
}

int entry()
{
    int watchdog_fd = -1;

    watchdog_fd = get_watchdog_fd();

    close(watchdog_fd);

    return 0;
}

int main()
{
    entry();
    return 0;
}
