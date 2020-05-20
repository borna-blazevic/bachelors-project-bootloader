/* main.c */
#include "main.h"
#include "memory_config.h"

static void prvSetupHardware(void);

int main(void)
{
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00000);
	prvSetupHardware();
	gpio_led_state(LED3_ORANGE_ID, 1);
	uint32_t *bootenv = (uint32_t *) &_shared;

	if (*bootenv != 1 && *bootenv != 2 && *bootenv != 3)
	{
		*bootenv = 1;
		print_string("Initial message\n");
	}
	if (*bootenv == 3)
	{
		print_string("booting updated\n");
		void **_app_start= (void **)&_new_image;
		((void (*)())_app_start[1])();
	}
	else if (*bootenv == 1 )
	{
		void **_app_start = (void **)&_recovery_image;
		((void (*)())_app_start[1])();
	}
	else if (*bootenv == 2 )
	{
		if(bootenv[55]){
			uint32_t *firmware_beggining =  (uint32_t *)&_firmware_upgrade;
			uint32_t *firmware_end = (uint32_t *)bootenv[50];
			uint32_t *flash_start = &_new_image;
			uint32_t *flash_end = flash_start + (firmware_end-firmware_beggining);

			unlock_flash();
			if(erase_flash_blocks((uint32_t)flash_start, (uint32_t)flash_end)){
				print_string("failed flash at erase\n");
				*bootenv = 1;
			}
			if(write_flash_and_check_blocks((uint32_t)flash_start, (uint32_t)flash_end, firmware_beggining)){
				print_string("failed flash at write\n");
				*bootenv = 1;
			}
			print_string("flashed and checked\n");
			bootenv[55] = 0;
			lock_flash();
		}
		void **_app_start = (void **)&_recovery_image;
		((void (*)())_app_start[1])();
	}
}

static void prvSetupHardware(void)
{
	gpio_init();
	init_communication();
}