# Linux Watchdog Timer (WDT)
## Introduction

The linux watchdog timer allows the linux kernel to reset the system if serious problems are detected.

It's composed of three different layers:
1. watchdog hardware (hardware)
2. watchdog driver (kernel)
3. watchdog daemon (userspace)

The watchdog driver makes available in the filesystem the corresponding
device file to make the link between the hardware and userspace.
By default, it's located in "/dev/watchdog".

watchdogd:
+ The watchdog daemon opens /dev/watchdog.

+ It keeps writing to the device file often enough to
  prevent the kernel from resetting. (at least once per minute).
  Each write delays the reboot time another minute.

+ If a system issue occurs (process table full, not enough free memory,
  temperature too high, file table overflow, ...) describes in man (8).
  The watchdog the daemon will stop writing to the device file.

+ After a minute of inactivity the watchdog hardware will cause the reset.

## Program Description:
Show hardware watchdog information.
The default device is /dev/watchdog.
Note that the number of supported watchdog features is hardware specific.

## More Info:
* [Watchdog Timer](https://en.wikipedia.org/wiki/Watchdog_timer) - Wikipedia
* [Watchdog API](https://www.kernel.org/doc/Documentation/watchdog/watchdog-api.txt) - Kernel.org
* [wdctl](http://man7.org/linux/man-pages/man8/wdctl.8.html) - man (8) wdctl

### Usage:

```
sudo ./watchdog
```

### Output:

```
Device:         /dev/watchdog
Identity:       iTCO_wdt [version 0]
Timeout:        30 seconds
Pre-timeout:    0 seconds
Timeleft:       30 seconds
Last boot:      Power-On-Reset
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
