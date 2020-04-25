/*
 * FreeRTOS Kernel V10.3.1
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */


/* Scheduler includes. */
#include "FreeRTOS.h"
#include "hw_types.h"
#include "hw_memmap.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "uart.h"
#include "lmi_flash.h"
#include "sysctl.h"
#include "string.h"

/* Delay between cycles of the 'check' task. */
#define mainUART_DELAY						( ( TickType_t ) 10000 / portTICK_PERIOD_MS )

/* UART configuration - note this does not use the FIFO so is not very
efficient. */
#define mainBAUD_RATE				( 19200 )
#define mainFIFO_SET				( 0x10 )
/*
 * Configure the processor and peripherals for this demo.
 */
static void prvSetupHardware( void );
static void vUARTTask( void *pvParameter );

/* String that is transmitted on the UART. */
static char *cMessage = "Hello world bootloader\n";
char pointer[10] = { 0 };
// char bootenv[14] __attribute__((at(0x16000))) = "Bootenv Test\n";
extern char _shared_data_start;
static volatile char *pcNextChar;
/*-----------------------------------------------------------*/

int main( void )
{
	/* Configure the clocks, UART and GPIO. */
	prvSetupHardware();

	char *bootenv = &_shared_data_start;

	if (*bootenv != 'T' && *bootenv != 'U')
	{
		memcpy(bootenv, "Bootloader sent initial message\n", 33);
	}else
	{
		printUART(bootenv);
	}
	if (*bootenv == 'U')
	{
		memcpy(bootenv, "Bootloader sent to updated hi\n", 31);
		extern void *_app_start2[];
		((void(*)())_app_start2[1])();
	}else
	{
		if(*bootenv == 'T')
			memcpy(bootenv, "RBootloader sent hi\n", 21);

		extern void *_app_start1[];
		((void(*)())_app_start1[1])();
	}


	/* Will only get here if there was insufficient heap to start the
	scheduler. */

	return 0;
}
/*-----------------------------------------------------------*/


void printUART(char *mes){
	
	while (*mes != '\0')
	{
		UARTCharPut(UART0_BASE, *mes);
		mes++;
	}
}

static void prvSetupHardware( void )
{
	SysCtlClockSet( SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ );
	SysCtlPeripheralEnable( SYSCTL_PERIPH_UART0 );
	UARTEnable( UART0_BASE );
}