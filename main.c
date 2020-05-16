/* main.c */
#include "main.h"
#include "memory_config.h"


static void prvSetupHardware( void );

int main(void)
{
	int i;
	char c;

	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00000);
	prvSetupHardware();
	char *bootenv = &_shared_data_start;

	if (*bootenv != 'T' && *bootenv != 'U')
	{
		print_string("Initial message\n");
		memcpy(bootenv, "Bootloader sent initial message\n", 33);
	}else
	{
		print_string(bootenv);
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
}


static void prvSetupHardware( void )
{
	gpio_init();
	init_communication();
}