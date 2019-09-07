#include <stdio.h>

#include "watchdog.h"


static void dump_watchdog_info(struct wdinfo *info)
{
    printf("Device:\t\t%s\n", WATCHDOG_PATHNAME);
    printf("Identity:\t%s [version %d]\n", info->identity, info->firmware_version);
    printf("Timeout: \t%d seconds\n", info->timeout);
    printf("Pre-timeout: \t%d seconds\n", info->pretimeout);
    printf("Timeleft: \t%d seconds\n", info->timeleft);
    printf("Last boot: \t%s\n", (info->bootstatus != 0) ? "Watchdog" : "Power-On-Reset");
}


int main()
{
    struct wdinfo info;
    if (wd_getinfo(&info) == -1) {
        printf("wd_getinfo() failed\n");
        return -1;
    }

    dump_watchdog_info(&info);

    return 0;
}

