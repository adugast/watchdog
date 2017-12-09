#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/watchdog.h>


/////////////////////////////////////////////////////////////////////
/*!
 *      == W A T C H D O G ==
 *
 * DESCRIPTION:
 *      Show hardware watchdog status. The default device is /dev/watchdog.
 *      Note that the number of supported watchdog features is hardware specific.
 *
 * INFO:
 *      https://www.kernel.org/doc/Documentation/watchdog/watchdog-api.txt
 *      man (8) wdctl
 *
 *///////////////////////////////////////////////////////////////////


#define WATCHDOG_PATHNAME "/dev/watchdog"

#define err(format, ...) \
    fprintf(stderr, format, __VA_ARGS__)


static int get_watchdog_fd(void)
{
    int fd = -1;

    fd = open(WATCHDOG_PATHNAME, O_RDWR);
    if (fd == -1) {
        err("Unable to open %s: %s.\n", WATCHDOG_PATHNAME, strerror(errno));
        return -1;
    }

    return fd;
}


static int watchdog(int fd)
{
    struct watchdog_info ident;
    int timeout = -1;
    int pretimeout = -1;
    int timeleft = -1;
    int bootstatus = -1;

    printf("Device:\t\t%s\n", WATCHDOG_PATHNAME);
    if (ioctl(fd, WDIOC_GETSUPPORT, &ident) == 0) {
        printf("Identity:\t%s [version %d]\n", ident.identity, ident.firmware_version);
    } else {
        err("Error: Cannot read watchdog identity: %s\n", strerror(errno));
    }

    /* Display current watchdog timeout */
    if (ioctl(fd, WDIOC_GETTIMEOUT, &timeout) == 0) {
        printf("Timeout: \t%d seconds\n", timeout);
    } else {
        err("Error: Cannot read watchdog timeout: %s\n", strerror(errno));
    }

    /* Display current watchdog pre-timeout */
    if (ioctl(fd, WDIOC_GETPRETIMEOUT, &pretimeout) == 0) {
        printf("Pre-timeout: \t%d seconds\n", pretimeout);
    } else {
        err("Error: Cannot read watchdog pretimeout: %s\n", strerror(errno));
    }

    /* Display current watchdog timeleft */
    if (ioctl(fd, WDIOC_GETTIMELEFT, &timeleft) == 0) {
        printf("Timeleft: \t%d seconds\n", timeleft);
    } else {
        err("Error: Cannot read watchdog timeleft: %s\n", strerror(errno));
    }

    /* Check if last boot is caused by watchdog */
    if (ioctl(fd, WDIOC_GETBOOTSTATUS, &bootstatus) == 0) {
        printf("Last boot: \t%s\n", (bootstatus != 0) ? "Watchdog" : "Power-On-Reset");
    } else {
        err("Error: Cannot read watchdog boot status: %s\n", strerror(errno));
    }

    return 0;
}


static int entry()
{
    int fd = -1;

    fd = get_watchdog_fd();
    if (fd == -1)
        return -1;

    watchdog(fd);

    /* The 'V' value needs to be written into watchdog device file to indicate
     * that we intend to close/stop the watchdog. Otherwise, debug message
     * 'Watchdog timer closed unexpectedly' will be printed in dmesg
     */
    write(fd, "V", 1);

    close(fd);

    return 0;
}


int main()
{
    entry();
    return 0;
}
