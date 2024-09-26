#include "wifi_hal_com.h"

void print_driver_version(void) {
    printk("driver compile date: 2024-09-26 20:09:24,driver hash: 151b129a1cb426ca3aa81c3406cd5ba7b0e86ff1\n");
    printk("fw compile date: 2024-09-26 20:09:01,fw hash: 1562a8c240ce94b6b99a03cebdbfa4487d234a80,fw size: 12492648\n");
    printk("rf cali: last commit: 2024-06-07 09:25:17 hash:ad714e20b62691182ced43c0606d2ff9f3644c73\n");
}
