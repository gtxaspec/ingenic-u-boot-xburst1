dir := tools/ingenic-tools/spl_params_fixer
obj := $(objtree)/$(dir)
src := $(srctree)/$(dir)

hostprogs-y += spl_params_fixer
spl_params_fixer-objs := spl_params_fixer.o

HOST_EXTRACFLAGS += -I$(srctree)/include/ingenic/ \
                    -I$(srctree)/arch/$(ARCH)/include/ \
                    -I$(srctree)/include/generated/ \
                    -include $(srctree)/include/configs/$(CONFIG_SYS_CONFIG_NAME).h

always := $(hostprogs-y)

