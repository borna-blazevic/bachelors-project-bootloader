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
    BOOTLOADER (RWX) : ORIGIN = 0x00000000, LENGTH = 16K
    RECOVERY_IMAGE (RWX) : ORIGIN = 0x00004000, LENGTH = 32K
    SHARED (rwx) : ORIGIN = 0x20000000, LENGTH = 1K
    NEW_IMAGE (RWX) : ORIGIN = 0x20000400, LENGTH = 32K
    SRAM (rwx) : ORIGIN = 0x2000C400, LENGTH = 31K
}

_shared_data_start = ORIGIN(SHARED);
_app_start1 = ORIGIN(RECOVERY_IMAGE);
_app_start2 = ORIGIN(NEW_IMAGE);

SECTIONS
{
    .text :
    {
        KEEP(*(.isr_vector))
        . = ALIGN(4);
            *(.text)
            *(.rodata*)
            _etext = .;
    } > BOOTLOADER

/* .data : AT (ADDR(.text) + SIZEOF(.text)) */
    .ARM.exidx :
    {
        *(.ARM.exidx*)
        *(.gnu.linkonce.armexidx.*)
    } > BOOTLOADER

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
