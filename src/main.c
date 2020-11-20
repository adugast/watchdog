#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "watchdog.h"


static void dump_watchdog_info(struct wdinfo *info)
{
    printf("Device:\t\t%s\n", WATCHDOG_PATHNAME);
    printf("Identity:\t%s [version %d]\n", info->identity, info->firmware_version);
    if (info->timeout != DATA_ERROR) printf("Timeout: \t%d seconds\n", info->timeout);
    if (info->pretimeout != DATA_ERROR) printf("Pre-timeout: \t%d seconds\n", info->pretimeout);
    if (info->timeleft != DATA_ERROR) printf("Timeleft: \t%d seconds\n", info->timeleft);
    printf("Last boot: \t%s\n", (info->bootstatus != 0) ? "Watchdog" : "Power-On-Reset");
}

static void print_help() {
    printf("Watchdog interaction tool. Userspace watchdog daemon needs to be stopped.\n");
    printf("No arguments: Print watchdog info\n");
    printf("[-h/--help]: Print this help message\n");
    printf("[-s/--set] T: Set watchdog timer timeout to T seconds.\n\n");
}

int main(int argc, char *argv[])
{
    if (argc == 1) {
        struct wdinfo info;
        if (wd_getinfo(&info) == -1) {
            printf("wd_getinfo() failed\n");
            return -1;
        }
        dump_watchdog_info(&info);
    }
    if (argc >= 2) {
        if ((strcmp(argv[1], "-s") == 0) || (strcmp(argv[1], "--set") == 0)) {
            // Convert next argument to integer
            int timeout;
            char *endptr;
            errno = 0;
            long val;
            val = strtol(argv[2], &endptr, 0);
            /* Check for various possible errors */
            if (errno != 0) {
                perror("strtol");
                return -1;
            }
            if (endptr == argv[2]) {
                fprintf(stderr, "No digits were found\n");
                return -1;
            }
            timeout = (int)val;

            printf("Trying to set timeout to %d seconds...\n", timeout);

            // Send argument to function
            if (wd_settimeout(timeout) < 0) {
                printf("wd_settimeout() failed\n");
                return -1;
            }

            printf("Timeout successfully changed.\n");
        }
        else {
            // Default
            print_help();
        }
    }



    return 0;
}

