#!/bin/sh
export ARCH=mips
export CROSS_COMPILE=mips-linux-uclibc-gnu-
make distclean
make isvp_t32_goat_msc_defconfig
make -j32
# riscv32-unknown-linux-gnu-objdump -D u-boot > u-boot.dump
# riscv32-unknown-linux-gnu-readelf -a u-boot > u-boot.elf
