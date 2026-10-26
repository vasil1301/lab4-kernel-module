# SPDX-License-Identifier: GPL-2.0
ifneq ($(KERNELRELEASE),)
# kbuild part of makefile
obj-m := hello.o
ccflags-y += -g
else
# normal makefile
KDIR ?= /lib/modules/$(shell uname -r)/build

default:
	$(MAKE) -C $(KDIR) M=$$PWD

clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean

checkpatch:
	$(KDIR)/scripts/checkpatch.pl -f --no-tree hello.c

.PHONY: default clean checkpatch
endif
