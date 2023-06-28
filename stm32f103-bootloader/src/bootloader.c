#include "bootloader.h"

#include "stm32f1xx.h"

void bootloader_app_run(void)
{
  FLASH->CR |= FLASH_CR_LOCK;

  __set_MSP(*(volatile uint32_t*) FLASH_APP_START_ADDR);

  ((void (*)(void)) (*(uint32_t*) (FLASH_APP_START_ADDR + 4)))();
}

void bootloader_reset_device(void)
{
  NVIC_SystemReset();
}
