# Project configuration
#######################
TARGET   = panel.bin
SUBDIRS  = src/ lib/STM32F10x_StdPeriph_Driver/src/ lib/CMSIS/Core/CM3/ lib/CMSIS/Core/CM3/startup/gcc/
INCLUDES = $(SUBDIRS)
DEFS = -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER
LDSCRIPT = ./stm32f100c8_flash.ld

# Manipulation with config
INCLUDES  := src/ lib/STM32F10x_StdPeriph_Driver/inc/ lib/CMSIS/Core/CM3/
SOURCES   := $(wildcard $(addsuffix *.s,$(SUBDIRS))) $(wildcard $(addsuffix *.c,$(SUBDIRS)))

CFLAGS  = -mcpu=cortex-m3 -mthumb -Wall -Os -mapcs-frame -D__thumb2__=1
CFLAGS += -msoft-float -gdwarf-2 -mno-sched-prolog -fno-hosted -mtune=cortex-m3
CFLAGS += -march=armv7-m -mfix-cortex-m3-ldrd -ffunction-sections -fdata-sections -nostdlib $(DEFS)
ASFLAGS = -mcpu=cortex-m3 -gdwarf-2
LDFLAGS = -Wl,-static -Wl,--start-group -nostdlib -lgcc -Wl,-script=$(LDSCRIPT)

include ../build-env.mk
include ../rules-stm.mk

## Default rule executed
all: $(OUTDIR)/$(TARGET)

## Clean Rule
clean:
	@-rm -rvf $(OUTDIR)/

.PHONY: clean
