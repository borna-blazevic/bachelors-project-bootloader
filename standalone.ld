/******************************************************************************
 *
 * standalone.ld - Linker script for applications using startup.c and
 *                 DriverLib.
 *
 * Copyright (c) 2005-2007 Luminary Micro, Inc.  All rights reserved.
 *
 * Software License Agreement
 *
 * Luminary Micro, Inc. (LMI) is supplying this software for use solely and
 * exclusively on LMI's microcontroller products.
 *
 * The software is owned by LMI and/or its suppliers, and is protected under
 * applicable copyright laws.  All rights are reserved.  Any use in violation
 * of the foregoing restrictions may subject the user to criminal sanctions
 * under applicable laws, as well as to civil liability for the breach of the
 * terms and conditions of this license.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * LMI SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * This is part of revision 1049 of the Stellaris Driver Library.
 *
 *****************************************************************************/

/*Changed according to http://roboticravings.blogspot.com/2018/07/freertos-on-cortex-m3-with-qemu.html so the project can be compiled"*/
MEMORY
{
    BOOTLOADER (rx) : ORIGIN = 0x00000000, LENGTH = 16K
    RECOVERY_IMAGE (rx) : ORIGIN = 0x00016000, LENGTH = 32K
    NEW_IMAGE (rwx) : ORIGIN = 0x00048000, LENGTH = 32K
    SHARED (rwx) : ORIGIN = 0x20000000, LENGTH = 1K
    SRAM (rwx) : ORIGIN = 0x20001000, LENGTH = 63K
}

_shared_data_start = ORIGIN(SHARED);
_app_start1 = 0x00000000 + 0x16000;
_app_start2 = 0x00000000 + 0x48000;

SECTIONS
{
    .text :
    {
        KEEP(*(.isr_vector))
        . = ALIGN(4);
            *(.text)
            *(.rodata*)
            _etext = .;
    } > FLASH

/* .data : AT (ADDR(.text) + SIZEOF(.text)) */
    .ARM.exidx :
    {
        *(.ARM.exidx*)
        *(.gnu.linkonce.armexidx.*)
    } > FLASH

    _begin_data = .;

    .data : AT ( _begin_data )
    {
        _data = .;
        *(vtable)
        *(.data)
        _edata = .;
    } > SRAM

    .bss :
    {
        _bss = .;
        *(.bss)
        *(COMMON)
        _ebss = .;
    } > SRAM
}
