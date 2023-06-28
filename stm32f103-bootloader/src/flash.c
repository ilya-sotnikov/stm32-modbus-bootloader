#include "flash.h"
#include "bootloader.h"

#include "stm32f1xx.h"

#include <stdbool.h>

#define FLASH_APP_PAGES_CNT (FLASH_APP_MAX_ADDR - FLASH_APP_START_ADDR) / FLASH_PAGE_SIZE

static inline bool flash_is_locked(void);
static inline bool flash_is_busy(void);
static flash_error_t flash_unlock(void);
static flash_error_t flash_erase_page(uint16_t page);
static uint8_t flash_read_byte(uint32_t address);
static uint16_t flash_read_hword(uint32_t address);
static flash_error_t flash_write_hword(uint32_t address, uint16_t data);

static inline bool flash_is_locked(void)
{
  return FLASH->CR & FLASH_CR_LOCK;
}

static inline bool flash_is_busy(void)
{
  return FLASH->SR & FLASH_SR_BSY;
}

static flash_error_t flash_unlock(void)
{
  if (!flash_is_locked())
    return FLASH_NOERR;

  FLASH->KEYR = 0x45670123;
  FLASH->KEYR = 0xCDEF89AB;

  return flash_is_locked() ? FLASH_ERR : FLASH_NOERR;
}

static flash_error_t flash_erase_page(uint16_t page)
{
  if (flash_unlock() == FLASH_ERR)
    return FLASH_ERR;

  while (flash_is_busy());

  FLASH->CR |= FLASH_CR_PER;

  FLASH->AR = FLASH_APP_START_ADDR + page * FLASH_PAGE_SIZE;
  FLASH->CR |= FLASH_CR_STRT;

  while (flash_is_busy());

  FLASH->CR &= ~FLASH_CR_PER;

  return FLASH_NOERR;
}

static uint8_t flash_read_byte(uint32_t address)
{
  return *(__IO uint8_t*) address;
}

static uint16_t flash_read_hword(uint32_t address)
{
  return *(__IO uint16_t*) address;
}

static flash_error_t flash_write_hword(uint32_t address, uint16_t data)
{
  if (flash_is_locked())
    return FLASH_ERR;

  while (flash_is_busy());

  FLASH->CR |= FLASH_CR_PG;

  *(__IO uint16_t*) address = data;

  while (flash_is_busy());

  FLASH->CR &= ~(FLASH_CR_PG);

  if (FLASH->SR & FLASH_SR_WRPRTERR) {
    FLASH->SR |= FLASH_SR_WRPRTERR;
    return FLASH_ERR;
  }

  if (FLASH->SR & FLASH_SR_PGERR) {
    FLASH->SR |= FLASH_SR_PGERR;
    return FLASH_ERR;
  }

  if (flash_read_hword(address) != data)
    return FLASH_ERR;

  return FLASH_NOERR;
}

flash_error_t flash_erase_app(void)
{
  for (uint16_t i = 0; i < FLASH_APP_PAGES_CNT; ++i) {
    if (flash_erase_page(i) == FLASH_ERR)
      return FLASH_ERR;
  }
  return FLASH_NOERR;
}

flash_error_t flash_write_app_data(uint32_t offset, uint8_t *data, uint8_t size)
{
  uint16_t hword;
  uint32_t address;

  for (uint8_t i = 0; i < size; i += 2) {
    address = FLASH_APP_START_ADDR + offset + i;
    hword = (uint16_t) (data[i] | (data[i + 1] << 8));
    if (flash_write_hword(address, hword) == FLASH_ERR)
      return FLASH_ERR;
  }

  return FLASH_NOERR;
}

flash_error_t flash_lock(void)
{
  if (flash_is_locked())
    return FLASH_NOERR;

  FLASH->CR |= FLASH_CR_LOCK;

  return flash_is_locked() ? FLASH_NOERR : FLASH_ERR;
}

uint8_t flash_get_app_checksum(uint32_t fw_size_bytes)
{
  uint8_t checksum = 0;

  for (uint32_t i = 0; i < fw_size_bytes; ++i)
    checksum += flash_read_byte(FLASH_APP_START_ADDR + i);

  return checksum;
}
