
#include <stdint.h>
#include <hw_memmap.h>
#include <hw_types.h>
#include <uart.h>
#include <sysctl.h>



void print_uint32(uint32_t *mes){
	int i;
	UARTCharPut(UART0_BASE, (*mes & 0xff000000) >> 24);
	UARTCharPut(UART0_BASE, (*mes & 0xff0000) >> 16);
	UARTCharPut(UART0_BASE, (*mes & 0xff00) >> 8);
	UARTCharPut(UART0_BASE, (*mes & 0xff));
	
}

void print_string(char *mes){
	
	while (*mes != '\0')
	{
		UARTCharPut(UART0_BASE, *mes);
		mes++;
	}
	
}
void print_uint8(uint8_t *mes){
	
	UARTCharPut(UART0_BASE, *mes);
	
}

void read_uint32(uint32_t *mes){
    *mes = UARTCharGet(UART0_BASE);
	*mes |= UARTCharGet(UART0_BASE)<<8;
	*mes |= UARTCharGet(UART0_BASE)<<16;
	*mes |= UARTCharGet(UART0_BASE)<<24;
}


void read_uint8(uint8_t *mes){
    *mes = UARTCharGet(UART0_BASE) & 0xff;
}

void init_uart_communication(){
	SysCtlClockSet( SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ );
	SysCtlPeripheralEnable( SYSCTL_PERIPH_UART0 );
	UARTEnable( UART0_BASE );
}