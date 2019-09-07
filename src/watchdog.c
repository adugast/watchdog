#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/watchdog.h>

#include "watchdog.h"


#define print_err(format, ...) \
    fprintf(stderr, format, __VA_ARGS__)


static int _get_watchdog_fd(void)
{
    int wfd = open(WATCHDOG_PATHNAME, O_RDWR);
    if (wfd == -1) {
        print_err("Unable to open %s: %s.\n", WATCHDOG_PATHNAME, strerror(errno));
        return -1;
    }

    return wfd;
}


static int _fill_watchdog_info(int wfd, struct wdinfo *info)
{
    /* Get watchdog identity */
    struct watchdog_info ident = {0};
    if (ioctl(wfd, WDIOC_GETSUPPORT, &ident) == -1)
        print_err("Error: Cannot read watchdog support: %s\n", strerror(errno));
    info->options = ident.options;
    info->firmware_version = ident.firmware_version;
    memcpy(info->identity, ident.identity, 32);

    /* Get watchdog current timeout */
    if (ioctl(wfd, WDIOC_GETTIMEOUT, info->timeout) == -1)
        print_err("Error: Cannot read watchdog timeout: %s\n", strerror(errno));
    /* Get watchdog current pre-timeout */
    if (ioctl(wfd, WDIOC_GETPRETIMEOUT, info->pretimeout) == -1)
        print_err("Error: Cannot read watchdog pretimeout: %s\n", strerror(errno));
    /* Get watchdog current timeleft */
    if (ioctl(wfd, WDIOC_GETTIMELEFT, info->timeleft) == -1)
        print_err("Error: Cannot read watchdog timeleft: %s\n", strerror(errno));
    /* Get watchdog current bootstatus */
    if (ioctl(wfd, WDIOC_GETBOOTSTATUS, info->bootstatus) == -1)
        print_err("Error: Cannot read watchdog boot status: %s\n", strerror(errno));

    return 0;
}


int wd_getinfo(struct wdinfo *info)
{
    int wfd = _get_watchdog_fd();
    if (wfd == -1)
        return -1;

    if (_fill_watchdog_info(wfd, info) == -1) {
        close(wfd);
        return -1;
    }

    /* The 'V' value needs to be written into watchdog device file to indicate
     * that we intend to close/stop the watchdog. Otherwise, debug message
     * 'Watchdog timer closed unexpectedly' will be printed in dmesg
     */
    if (write(wfd, "V", 1) != 1)
        print_err("%s", "Watchdog closed unexpectedly\n");

    close(wfd);

    return 0;
}

