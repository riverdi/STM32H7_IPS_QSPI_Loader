/*
 * Dev_Inf.c
 *
 */
#include "Dev_Inf.h"
#include "quadspi.h"

/* This structure contains information used by ST-LINK Utility to program and erase the device */

#ifdef MICRON_QSPI
struct StorageInfo const StorageInfo = {



    "QSPI_STM32H747I-Riverdi-MICRON",                          // Device Name + version number
    NOR_FLASH,                          // Device Type
    0x90000000,                         // Device Start Address
    MEMORY_FLASH_SIZE,                  // Device Size in Bytes
    MEMORY_PAGE_SIZE,                   // Programming Page Size
    0xFF,                               // Initial Content of Erased Memory

    // Specify Size and Address of Sectors (view example below)
    {   {
            (MEMORY_FLASH_SIZE / MEMORY_SECTOR_SIZE),  // Sector Numbers,
            (uint32_t) MEMORY_SECTOR_SIZE
        },       //Sector Size

        { 0x00000000, 0x00000000 }
    }
};
#else

struct StorageInfo const StorageInfo = {



    "QSPI_STM32H747I-Riverdi-GD",                          // Device Name + version number
    NOR_FLASH,                          // Device Type
    0x90000000,                         // Device Start Address
    MEMORY_FLASH_SIZE,                  // Device Size in Bytes
    MEMORY_PAGE_SIZE,                   // Programming Page Size
    0xFF,                               // Initial Content of Erased Memory

    // Specify Size and Address of Sectors (view example below)
    {   {
            (MEMORY_FLASH_SIZE / MEMORY_SECTOR_SIZE),  // Sector Numbers,
            (uint32_t) MEMORY_SECTOR_SIZE
        },       //Sector Size

        { 0x00000000, 0x00000000 }
    }
};
#endif

