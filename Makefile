MODULE_NAME = STM32F407

#STLINK path
STLINK = $(whereis st-flash)

#Toolchain
PREFIX = arm-none-eabi-

CC = $(PREFIX)gcc
AS = $(PREFIX)gcc
LD = $(PREFIX)gcc
BIN = $(PREFIX)objcopy

CFLAGS = -Wall \
         -mcpu=cortex-m4 \
         -mthumb \
         -mfloat-abi=softfp \
         -fomit-frame-pointer \
         -fno-strict-aliasing \
         -specs=nosys.specs \
         -specs=nano.specs \
		 -Og	\
         -g -std=c99

# Assembler flags
AFLAGS = -x assembler-with-cpp

# Linker flags
LDFLAGS = -specs=nosys.specs -specs=nano.specs -mcpu=cortex-m4 -mthumb

#Binary flags
BINFLAGS = -O binary

# Libraries and Startup files
SPL = Libraries/STM32F4xx_StdPeriph_Driver
CMSIS = Libraries/CMSIS
STARTUP = $(CMSIS)/Device/ST/STM32F4xx/Source/Templates/gcc_ride7
FREERTOS = FreeRTOS


# Includes
INC =  -I $(SPL)/inc
INC += -I $(CMSIS)/Include
INC += -I $(CMSIS)/Device/ST/STM32F4xx/Include
INC += -I $(FREERTOS)/include
INC += -I $(FREERTOS)/portable/ARM_CM4F
INC += -I ./include
INC += -I .

CFLAGS += $(INC)
CFLAGS += -D USE_STDPERIPH_DRIVER -D STM32F40xx -D USE_STM32F07_DISCO

BLACKLIST = $(SPL)/src/stm32f4xx_fmc.c
BLACKLIST += $(SPL)/src/stm32f4xx_can.c
BLACKLIST += $(SPL)/src/stm32f4xx_adc.c
BLACKLIST += $(SPL)/src/stm32f4xx_dac.c
BLACKLIST += $(SPL)/src/stm32f4xx_dsip.c
BLACKLIST += $(SPL)/src/stm32f4xx_cryp.c
BLACKLIST += $(SPL)/src/stm32f4xx_cryp_aes.c
BLACKLIST += $(SPL)/src/stm32f4xx_cryp_des.c
BLACKLIST += $(SPL)/src/stm32f4xx_cryp_tdes.c
BLACKLIST += $(SPL)/src/stm32f4xx_dbgmcu.c
BLACKLIST += $(SPL)/src/stm32f4xx_hash.c
BLACKLIST += $(SPL)/src/stm32f4xx_hash_md5.c
BLACKLIST += $(SPL)/src/stm32f4xx_hash_sha1.c
BLACKLIST += $(SPL)/src/stm32f4xx_rng.c
BLACKLIST += $(SPL)/src/stm32f4xx_i2c.c
BLACKLIST += $(SPL)/src/stm32f4xx_sdio.c
BLACKLIST += $(SPL)/src/stm32f4xx_spdifrx.c
BLACKLIST += $(SPL)/src/stm32f4xx_pwr.c
BLACKLIST += $(SPL)/src/stm32f4xx_spi.c
BLACKLIST += $(SPL)/src/stm32f4xx_qspi.c
BLACKLIST += $(SPL)/src/stm32f4xx_dsi.c
BLACKLIST += $(SPL)/src/stm32f4xx_sai.c
BLACKLIST += $(SPL)/src/stm32f4xx_ltdc.c
BLACKLIST += $(SPL)/src/stm32f4xx_lptim.c
BLACKLIST += $(SPL)/src/stm32f4xx_exti.c
BLACKLIST += $(SPL)/src/stm32f4xx_iwdg.c
BLACKLIST += $(SPL)/src/stm32f4xx_cec.c
BLACKLIST += $(SPL)/src/stm32f4xx_dma.c

_SRC = $(wildcard *.c)
_SRC += $(wildcard $(CMSIS)/Device/ST/STM32F4xx/Templates/*.c)
_SRC += $(wildcard $(SPL)/src/*.c)
_SRC += $(wildcard $(FREERTOS)/*.c)
_SRC += $(wildcard $(FREERTOS)/portable/MemMang/*.c)
_SRC += $(wildcard $(FREERTOS)/portable/ARM_CM4F/*.c)
_SRC += $(wildcard ./src/*.c)
SRC  = $(filter-out $(BLACKLIST), $(_SRC))

ASRC = $(STARTUP)/startup_stm32f40xx.s

OBJ = $(patsubst %.c,%.o,$(SRC))
AOBJ = $(patsubst %.s,%.o,$(ASRC))

LINKER_SCRIPT = STM32F407VGTx_FLASH.ld

.PRECIOUS: %.c %.o

all: app.bin

%.o: %.c 
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(MODULE_NAME): Compiled $< successfully!"

%.o : %.s 
	@$(AS) $(CFLAGS) $(AFLAGS) -c $< -o $@
	@echo "$(MODULE_NAME): Compiled "$<" successfully!"

app.bin : app.elf
	@$(BIN) $< $(BINFLAGS) $@
	@echo "$(MODULE_NAME): Generated binary successfully!"

app.elf : $(AOBJ) $(OBJ)
	@$(LD) $(AOBJ) $(OBJ) \
	$(LDFLAGS) -o $@ \
	-lc  \
	-T$(LINKER_SCRIPT)
	@echo "$(MODULE_NAME): Linked app successfully!"

clean :
	@rm -f app.elf app.bin
	@rm $(OBJ)

burn: app.bin
	@st-flash write app.bin 0x8000000

