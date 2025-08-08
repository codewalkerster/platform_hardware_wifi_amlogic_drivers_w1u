#include "wifi_hal_com.h"

void print_driver_version(void) {
    printk("driver compile date: 2025-08-08 15:38:46,driver hash: 0ccf0c1186e2006de1143a5f489a022f0a7f2c90\n");
    printk("fw compile date: 2025-08-08 15:37:53,fw hash: 7bca76c312a5dfc6bfeb8b31b3890726f6bae928,fw size: 147456\n");
    printk("rf cali: last commit: 2025-08-07 14:04:58 hash:7bca76c312a5dfc6bfeb8b31b3890726f6bae928\n");
}
