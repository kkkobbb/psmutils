ABS_PWD := $(shell pwd -P)
DIR_KERNEL := /lib/modules/$(shell uname -r)/build
obj-m := mcac.o mcaps1m.o

all:
	make -C $(DIR_KERNEL) M=$(ABS_PWD) modules

clean:
	make -C $(DIR_KERNEL) M=$(ABS_PWD) clean

