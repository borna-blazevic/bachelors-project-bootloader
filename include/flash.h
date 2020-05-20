#ifndef FLASH_H
#define FLASH_H

#include <stdint.h>

void unlock_flash();
void lock_flash();
int erase_flash_blocks(uint32_t flash_erase_start, uint32_t flash_erase_end);
int write_flash_and_check_blocks(uint32_t flash_write_start, uint32_t flash_write_end, uint32_t *data);

#endif
