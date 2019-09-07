#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__


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


#ifdef __cplusplus
extern "C"
{
#endif


#include <stdint.h>


#define WATCHDOG_PATHNAME "/dev/watchdog"


struct wdinfo {
    uint32_t options;           /* Options the card/driver support */
    uint32_t firmware_version;  /* Firmware version of the card */
    uint8_t identity[32];       /* Identity of the board */
    int timeout;
    int pretimeout;
    int timeleft;               /* Number of seconds before reboot */
    int bootstatus;             /* Status of the last reboot */
};


/*!
 * \brief Get watchdog info
 *
 * Fill a "struct wdinfo" with information on current watchdog hardware.
 * default path: "/dev/watchdog"
 *
 * \param   info    OUT struct wdinfo filled
 *
 * \return -1 on error, 0 on success
 */
int wd_getinfo(struct wdinfo *info);


#ifdef __cplusplus
}
#endif


#endif /* __WATCHDOG_H__ */

