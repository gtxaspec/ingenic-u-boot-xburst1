
dir := tools/ingenic-tools/clk_params_creator
obj := $(objtree)/$(dir)
src := $(srctree)/$(dir)

hostprogs-y += clk_params_creator
clk_params_creator-objs := clk_params_creator.o

HOST_EXTRACFLAGS += -I$(srctree)/include/ingenic/	\
		    -I$(srctree)/arch/$(ARCH)/include/	\
		    -I$(srctree)/include/generated/	\
		    -include $(srctree)/include/configs/$(CONFIG_SYS_CONFIG_NAME).h

always := $(hostprogs-y) clk_reg_values.h

$(obj)/clk_reg_values.h: $(obj)/clk_params_creator
	@$(obj)/clk_params_creator > $(objtree)/include/generated/$(notdir $@)
