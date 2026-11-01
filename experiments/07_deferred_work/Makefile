ifneq ($(KERNELRELEASE),)
obj-m := deferred_work.o
deferred_work-objs := src/deferred_work.o
else
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

install:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	sudo insmod deferred_work.ko

uninstall:
	sudo rmmod deferred_work

endif