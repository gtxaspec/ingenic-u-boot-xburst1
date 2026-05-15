
dir := tools/ingenic-tools/ddr_creator_chip_v3
obj := $(objtree)/$(dir)
src := $(srctree)/$(dir)


ddr-type-y = ddr_params_creator.o
ddr-type-y += ddrp_params_wrap.o
ddr-type-y += supported_ddr_chips.o
ddr-type-y += dump_ddr_params.o
ddr-type-$(CONFIG_DDR_TYPE_DDR2) += ddr2_params.o
ddr-type-$(CONFIG_DDR_TYPE_DDR3) += ddr3_params.o
ddr-type-$(CONFIG_DDR_TYPE_LPDDR3) += lpddr3_params.o
src_file := $(ddr-type-y)

hostprogs-y := ddr_params_creator
ddr_params_creator-objs := $(src_file)

HOST_EXTRACFLAGS += -I$(srctree)/include/ingenic/	\
		    -I$(srctree)/arch/$(ARCH)/include/	\
		    -I$(srctree)/include/generated/	\
		    -include $(srctree)/include/configs/$(CONFIG_SYS_CONFIG_NAME).h

always := $(hostprogs-y) ddr_reg_values.h

$(obj)/ddr_reg_values.h: $(obj)/ddr_params_creator
	@$(obj)/ddr_params_creator > $(objtree)/include/generated/$(notdir $@)
