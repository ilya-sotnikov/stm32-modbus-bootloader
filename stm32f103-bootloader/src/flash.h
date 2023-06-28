#ifndef FLASH_H
#define FLASH_H

#include <stdint.h>

typedef enum {
  FLASH_NOERR, FLASH_ERR
} flash_error_t;

flash_error_t flash_erase_app(void);
flash_error_t flash_write_app_data(uint32_t offset, uint8_t *data, uint8_t size);
uint8_t flash_get_app_checksum(uint32_t fw_size_bytes);
flash_error_t flash_lock(void);

#endif /* FLASH_H */
