dir := tools/ingenic-tools/spl_checksum
obj := $(objtree)/$(dir)
src := $(srctree)/$(dir)

hostprogs-y += spl_checksum
spl_checksum-objs := spl_checksum.o

HOST_EXTRACFLAGS += -I$(srctree)/include/ingenic/ \
                    -I$(srctree)/arch/$(ARCH)/include/ \
                    -I$(srctree)/include/generated/ \
                    -include $(srctree)/include/configs/$(CONFIG_SYS_CONFIG_NAME).h

always := $(hostprogs-y)

