#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include "stm32f1xx.h"

#define FLASH_PAGE_SIZE (0x400)
#define FLASH_APP_START_ADDR (FLASH_BASE + 0x00004400)
#define FLASH_APP_MAX_ADDR (FLASH_BASE + 0x00010000)

void bootloader_app_run(void);
void bootloader_reset_device(void);

#endif /* BOOTLOADER_H */
