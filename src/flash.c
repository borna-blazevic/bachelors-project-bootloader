#include <stdint.h>
#include <stm32f4xx.h>
#include <stm32f4xx_flash.h>

#define ADDR_FLASH_SECTOR_0      ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_1      ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_2      ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_3      ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_4      ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes   */
#define ADDR_FLASH_SECTOR_5      ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_6      ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_7      ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_8      ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_9      ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_10     ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11     ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */

static uint32_t GetSector(uint32_t Address);

void unlock_flash()
{
    FLASH_Unlock();
}

void lock_flash()
{
    FLASH_Lock();
}
int erase_flash_blocks(uint32_t flash_erase_start, uint32_t flash_erase_end)
{
    uint32_t uwStartSector = 0;
    uint32_t uwEndSector = 0;
    uint32_t uwSectorCounter = 0;

    uwStartSector = GetSector(flash_erase_start);
    uwEndSector = GetSector(flash_erase_end);

    /* Strat the erase operation */
    uwSectorCounter = uwStartSector;
    while (uwSectorCounter <= uwEndSector)
    {
        if (FLASH_EraseSector(uwSectorCounter, VoltageRange_3) != FLASH_COMPLETE)
        {
            return -1;
        }

        if (uwSectorCounter == FLASH_Sector_11)
        {
            uwSectorCounter += 40;
        }
        else
        {
            uwSectorCounter += 8;
        }
    }
    return 0;
}

int write_flash_and_check_blocks(uint32_t flash_write_start, uint32_t flash_write_end, uint32_t *data)
{

    int i = 0;
    uint32_t uwAddress = flash_write_start;

    __IO uint32_t uwData32 = 0;
    __IO uint32_t uwMemoryProgramStatus = 0;

    while (uwAddress < flash_write_end)
    {
        if (FLASH_ProgramWord(uwAddress, data[i]) == FLASH_COMPLETE)
        {
            i++;
            uwAddress = uwAddress + 4;
        }
        else
        {
            return -1;
        }
    }

    uwAddress = flash_write_start;
    uwMemoryProgramStatus = 0;
    i = 0;

    while (uwAddress < flash_write_end)
    {
        uwData32 = *(__IO uint32_t *)uwAddress;

        if (uwData32 != data[i])
        {
            uwMemoryProgramStatus++;
        }

        uwAddress = uwAddress + 4;
        i++;
    }

    return uwMemoryProgramStatus;
}

static uint32_t GetSector(uint32_t Address)
{
    uint32_t sector = 0;

    if ((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
    {
        sector = FLASH_Sector_0;
    }
    else if ((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
    {
        sector = FLASH_Sector_1;
    }
    else if ((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
    {
        sector = FLASH_Sector_2;
    }
    else if ((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
    {
        sector = FLASH_Sector_3;
    }
    else if ((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
    {
        sector = FLASH_Sector_4;
    }
    else if ((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
    {
        sector = FLASH_Sector_5;
    }
    else if ((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
    {
        sector = FLASH_Sector_6;
    }
    else if ((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
    {
        sector = FLASH_Sector_7;
    }
    else if ((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
    {
        sector = FLASH_Sector_8;
    }
    else if ((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
    {
        sector = FLASH_Sector_9;
    }
    else if ((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
    {
        sector = FLASH_Sector_10;
    }
    else /*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_23))*/
    {
        sector = ADDR_FLASH_SECTOR_11;
    }
    return sector;
}