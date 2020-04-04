# pocketstation (arm7tdmi)
#
PROG_NAME := sample
LDMAP := $(PROG_NAME).1.map
EXE_ELF := $(PROG_NAME).2.elf
BINARY := $(PROG_NAME).3.bin
MCDATA := $(PROG_NAME).4.ps
MCDATA_FROM := from

SRCS := init.c sub.c main.c utils.c
OBJS := asm/header.o asm/start.o asm/handler.o $(SRCS:.c=.o)
LDSCRIPT := ../etc/ldscripts/arm_pocketstation.x
SCNAME := scname.txt

export PKS_DIR := ${HOME}/pks_toolchain/
export PKS_PREFIX := $(PKS_DIR)bin/arm-none-elf-

# toolchain
CC := $(PKS_PREFIX)gcc -mcpu=arm7tdmi
OBJCOPY := $(PKS_PREFIX)objcopy
LD := $(PKS_PREFIX)ld
# other tools
TOOLS_DIR := ../../psmedit
TO_MC := $(TOOLS_DIR)/psmedit


# compile options
CFLAGS := -O2 -nostdinc -nostdlib
#ASFLAGS := -Wa,--fix-v4bx
ASFLAGS :=
export FLOAT := soft
ifeq ($(FLOAT), soft)
  # soft float
  CFLAGS += -msoft-float
  ASFLAGS += -Wa,-mfloat-abi=soft
  LIBDIR += -L$(PKS_DIR)/lib/gcc/arm-none-elf/3.4.6
else
  # hard float
  CFLAGS += -mhard-float
  ASFLAGS += -Wa,-mfloat-abi=hard,-mfpu=fpa
  LIBDIR += -L$(PKS_DIR)/lib/gcc/arm-none-elf/3.4.6/fpu
endif
LDFLAGS := $(LIBDIR) -lgcc


.PHONY: all clean cleanlib cleanall

all: $(MCDATA)

$(EXE_ELF): $(OBJS)
	$(LD) -Map $(LDMAP) -o $@ -T$(LDSCRIPT) $(LDFLAGS) $^

$(BINARY): $(EXE_ELF)
	$(OBJCOPY) -O binary $^ $@
	@printf "\n"
	@du -b $(BINARY)
	@printf "\n"

$(MCDATA): $(BINARY) $(SCNAME)
	$(TO_MC) -f -W $< | $(TO_MC) -T $$(cat $(SCNAME)) -o $@

$(MCDATA_FROM): $(MCDATA)
	cp $^ $@

clean:
	rm -f $(OBJS) $(LDMAP) $(EXE_ELF) $(BINARY) $(MCDATA) $(MCDATA_FROM)
