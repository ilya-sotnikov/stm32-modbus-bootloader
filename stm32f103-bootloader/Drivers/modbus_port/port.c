#include <stdint.h>
#include "cmsis_gcc.h"

static uint16_t lock_nesting_count = 0;

void EnterCriticalSection(void)
{
  __disable_irq();
  ++lock_nesting_count;
}

void ExitCriticalSection(void)
{
  --lock_nesting_count;
  if (lock_nesting_count == 0) {
    __enable_irq();
  }
}
