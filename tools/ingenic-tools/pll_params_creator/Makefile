
dir := tools/ingenic-tools/pll_params_creator
obj := $(objtree)/$(dir)
src := $(srctree)/$(dir)

hostprogs-y += pll_params_creator
pll_params_creator-objs := pll_params_creator.o

HOST_EXTRACFLAGS += -I$(srctree)/include/ingenic/	\
		    -I$(srctree)/arch/$(ARCH)/include/	\
		    -I$(srctree)/include/generated/	\
		    -include $(srctree)/include/configs/$(CONFIG_SYS_CONFIG_NAME).h

always := $(hostprogs-y) pll_reg_values.h

$(obj)/pll_reg_values.h: $(obj)/pll_params_creator
	@$(obj)/pll_params_creator > $(objtree)/include/generated/$(notdir $@)
