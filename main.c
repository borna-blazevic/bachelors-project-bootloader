/* main.c */
#include "main.h"
#include "memory_config.h"

static void prvSetupHardware(void);

int main(void)
{
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00000);
	prvSetupHardware();
	uint32_t *bootenv = (uint32_t *)&_shared;
	bootenv[6] = 0x4a4f4242;
	if (bootenv[3] != 1 && bootenv[3] != 2)
	{
		bootenv[3] = 1;
		print_string("Initial message\n");
		void **_app_start = (void **)&_recovery_image;
		((void (*)())_app_start[1])();
	}
	else if (bootenv[3] == 1)
	{

		if (bootenv[4] == 1)
		{
			uint32_t *firmware_beggining = (uint32_t *)&_firmware_upgrade;
			uint32_t *firmware_end = (uint32_t *)bootenv[5];
			uint32_t *flash_start = &_new_image;
			uint32_t *flash_end = flash_start + (firmware_end - firmware_beggining);

			unlock_flash();
			if (erase_flash_blocks((uint32_t)flash_start, (uint32_t)flash_end))
			{
				print_string("\nfailed flash at erase\n");
				bootenv[3] = 1;
				print_string("booting recovery\n");
				void **_app_start = (void **)&_recovery_image;
				((void (*)())_app_start[1])();
			}
			if (write_flash_and_check_blocks((uint32_t)flash_start, (uint32_t)flash_end, firmware_beggining))
			{
				print_string("\nfailed flash at write\n");
				bootenv[3] = 1;
				print_string("booting recovery\n");
				void **_app_start = (void **)&_recovery_image;
				((void (*)())_app_start[1])();
			}
			print_string("\nflashed and checked\n");
			bootenv[4] = 0;
			lock_flash();
			print_string("booting updated\n");
			void **_app_start = (void **)&_new_image;
			((void (*)())_app_start[1])();
		}
		else
		{
			print_string("\nbooting recovery\n");
			void **_app_start = (void **)&_recovery_image;
			((void (*)())_app_start[1])();
		}
	}
	else if (bootenv[3] == 2)
	{
		if (bootenv[4] == 1)
		{
			uint32_t *firmware_beggining = (uint32_t *)&_firmware_upgrade;
			uint32_t *firmware_end = (uint32_t *)bootenv[5];
			uint32_t *flash_start = &_new_image;
			uint32_t *flash_end = flash_start + (firmware_end - firmware_beggining);

			unlock_flash();
			if (erase_flash_blocks((uint32_t)flash_start, (uint32_t)flash_end))
			{
				print_string("\nfailed flash at erase\n");
				bootenv[3] = 1;
				print_string("booting recovery\n");
				void **_app_start = (void **)&_recovery_image;
				((void (*)())_app_start[1])();
			}
			if (write_flash_and_check_blocks((uint32_t)flash_start, (uint32_t)flash_end, firmware_beggining))
			{
				print_string("\nfailed flash at write\n");
				bootenv[3] = 1;
				print_string("booting recovery\n");
				void **_app_start = (void **)&_recovery_image;
				((void (*)())_app_start[1])();
			}
			print_string("\nflashed and checked\n");
			bootenv[4] = 0;
			lock_flash();
			print_string("booting updated\n");
			bootenv[3] = 1;
			void **_app_start = (void **)&_new_image;
			((void (*)())_app_start[1])();
		}
		else
		{
			for (int i = 0; i < bootenv[0]; i++)
			{
				if ((bootenv[1] >> i) && (bootenv[2] >> i))
				{
					print_string("\nmistake happened\n");
					bootenv[3] = 1;
					print_string("booting recovery\n");
					void **_app_start = (void **)&_recovery_image;
					((void (*)())_app_start[1])();
				}
			}
			print_string("\nbooting updated\n");
			bootenv[3] = 1;
			void **_app_start = (void **)&_new_image;
			((void (*)())_app_start[1])();
		}
	}
}

static void prvSetupHardware(void)
{
	gpio_init();
	init_communication();
}