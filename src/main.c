/*!
 *      == L I N U X   W A T C H D O G   T I M E R ==
 *
 * The linux watchdog timer allows the linux kernel to
 * reset the system if serious problems are detected.
 *
 * It's composed of three different layers:
 *      1) watchdog hardware (hardware)
 *      2) watchdog driver (kernel)
 *      3) watchdog daemon (userspace)
 *
 * The watchdog driver makes available in the filesystem the corresponding
 * device file to make the link between the hardware and userspace.
 * By default, it's located in "/dev/watchdog".
 *
 * watchdogd:
 *      - The watchdog daemon opens /dev/watchdog.
 *
 *      - It keeps writing to the device file often enough to
 *      prevent the kernel from resetting. (at least once per minute).
 *      Each write delays the reboot time another minute.
 *
 *      - If a system issue occurs (process table full, not enough free memory,
 *      temperature too high, file table overflow, ...) describes in man (8).
 *      The watchdog the daemon will stop writing to the device file.
 *
 *      - After a minute of inactivity the watchdog hardware will cause the reset.
 *
 * PROGRAM DESCRIPTION:
 *      Show hardware watchdog status. The default device is /dev/watchdog.
 *      Note that the number of supported watchdog features is hardware specific.
 *
 * MORE INFO:
 *      https://en.wikipedia.org/wiki/Watchdog_timer
 *      https://www.kernel.org/doc/Documentation/watchdog/watchdog-api.txt
 *      man (8) wdctl
 *
 */


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/watchdog.h>


#define WATCHDOG_PATHNAME "/dev/watchdog"

#define print_err(format, ...) \
    fprintf(stderr, format, __VA_ARGS__)


static int get_watchdog_fd(void)
{
    int fd = open(WATCHDOG_PATHNAME, O_RDWR);
    if (fd == -1) {
        print_err("Unable to open %s: %s.\n", WATCHDOG_PATHNAME, strerror(errno));
        return -1;
    }

    return fd;
}


static void print_watchdog_status(int fd)
{
    /* Display watchdog device path */
    printf("Device:\t\t%s\n", WATCHDOG_PATHNAME);

    /* Display watchdog identity */
    struct watchdog_info ident = {0};
    if (ioctl(fd, WDIOC_GETSUPPORT, &ident) == 0) {
        printf("Identity:\t%s [version %d]\n", ident.identity, ident.firmware_version);
    } else {
        print_err("Error: Cannot read watchdog identity: %s\n", strerror(errno));
    }

    /* Display current watchdog timeout */
    int timeout = -1;
    if (ioctl(fd, WDIOC_GETTIMEOUT, &timeout) == 0) {
        printf("Timeout: \t%d seconds\n", timeout);
    } else {
        print_err("Error: Cannot read watchdog timeout: %s\n", strerror(errno));
    }

    /* Display current watchdog pre-timeout */
    int pretimeout = -1;
    if (ioctl(fd, WDIOC_GETPRETIMEOUT, &pretimeout) == 0) {
        printf("Pre-timeout: \t%d seconds\n", pretimeout);
    } else {
        print_err("Error: Cannot read watchdog pretimeout: %s\n", strerror(errno));
    }

    /* Display current watchdog timeleft */
    int timeleft = -1;
    if (ioctl(fd, WDIOC_GETTIMELEFT, &timeleft) == 0) {
        printf("Timeleft: \t%d seconds\n", timeleft);
    } else {
        print_err("Error: Cannot read watchdog timeleft: %s\n", strerror(errno));
    }

    /* Check if last boot is caused by watchdog */
    int bootstatus = -1;
    if (ioctl(fd, WDIOC_GETBOOTSTATUS, &bootstatus) == 0) {
        printf("Last boot: \t%s\n", (bootstatus != 0) ? "Watchdog" : "Power-On-Reset");
    } else {
        print_err("Error: Cannot read watchdog boot status: %s\n", strerror(errno));
    }

    return;
}


static int entry()
{
    int fd = get_watchdog_fd();
    if (fd == -1)
        return -1;

    print_watchdog_status(fd);

    /* The 'V' value needs to be written into watchdog device file to indicate
     * that we intend to close/stop the watchdog. Otherwise, debug message
     * 'Watchdog timer closed unexpectedly' will be printed in dmesg
     */
    if (write(fd, "V", 1) != 1)
        print_err("%s", "Watchdog closed unexpectedly\n");

    close(fd);

    return 0;
}


int main()
{
    entry();
    return 0;
}
