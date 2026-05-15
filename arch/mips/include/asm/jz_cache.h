/*
 * arch/mips/include/asm/jz_cache.h
 *
 * Copyright (c) 2016 Ingenic Semiconductor Co.,Ltd
 * 	qipengzhen <aric.pzqi@ingenic.com>
 * 	<porting from kernel arch/mips/include/asm/r4kcache.h>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __JZ_CACHE_H
#define __JZ_CACHE_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <asm/addrspace.h>


/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/***
 * Cache ops Definition.
 * */
#define Index_Invalidate_I      0x00
#define Index_Writeback_Inv_D   0x01
#define Index_Load_Tag_I        0x04
#define Index_Load_Tag_D        0x05
#define Index_Store_Tag_I       0x08
#define Index_Store_Tag_D       0x09
#define Index_Store_Data_I	    0x0c
#define Hit_Invalidate_I        0x10
#define Hit_Invalidate_D        0x11
#define Hit_Writeback_Inv_D     0x15

/*
 *  * R4000SC and R4400SC-specific cacheops
 *   */
#define Index_Invalidate_SI     0x02
#define Index_Writeback_Inv_SD  0x03
#define Index_Load_Tag_SI       0x06
#define Index_Load_Tag_SD       0x07
#define Index_Store_Tag_SI      0x0A
#define Index_Store_Tag_SD      0x0B
#define Create_Dirty_Excl_SD    0x0f
#define Hit_Invalidate_SI       0x12
#define Hit_Invalidate_SD       0x13
#define Hit_Writeback_Inv_SD    0x17
#define Hit_Writeback_SD        0x1b
#define Hit_Set_Virtual_SI      0x1e
#define Hit_Set_Virtual_SD      0x1f


#ifndef CONFIG_BOARD_L2CACHE
 /* There is no L2CACHE ops, such as X1000, define These Macros to avoid Compile Error*/
 #define CONFIG_SYS_L2CACHE_SIZE          (128 * 1024)
 #define CONFIG_SYS_L2CACHELINE_SIZE      (64)
 #define CONFIG_SYS_L2CACHE_WAYS          (8)
#endif


#define INDEX_BASE      KSEG0
#define PAGE_SIZE	(4096)

/* hardware special barrier */
#define __sync()                                \
        __asm__ __volatile__(                   \
                ".set   push		\n\t"   \
                ".set   noreorder	\n\t"   \
                ".set   mips2		\n\t"   \
                "sync			\n\t"   \
                ".set   pop		\n\t"   \
                : /*  no output */              \
                : /*  no input */               \
                : "memory")

#define INVALIDATE_BTB()                        \
	do {                                    \
	} while (0)

#define SYNC_WB() __sync()


#define __fast_iob()                            \
        __asm__ __volatile__(                   \
                ".set   push		\n\t"   \
                ".set   noreorder	\n\t"   \
                "lw     $0,%0		\n\t"   \
                "nop			\n\t"   \
                ".set   pop		\n\t"   \
                : /*  no output */              \
                : "m" (*(int *)CKSEG1)          \
                : "memory")


#define cache_op(op,addr)                   	 \
        __asm__ __volatile__(                    \
        "       .set    push           \n\t"     \
        "       .set    noreorder      \n\t"     \
        "       .set    mips3\n\t      \n\t"     \
        "       cache   %0, %1         \n\t"     \
        "       .set    pop            \n\t"     \
        :                                        \
        : "i" (op), "R" (*(unsigned char *)(addr)))

#define __iflush_prologue
#define __iflush_epilogue       INVALIDATE_BTB();
#define __dflush_prologue
#define __dflush_epilogue       SYNC_WB();
#define __inv_dflush_prologue
#define __inv_dflush_epilogue   SYNC_WB();
#define __sflush_prologue {
#define __sflush_epilogue }
#define __inv_sflush_prologue {
#define __inv_sflush_epilogue }


/****************************************************************************
 * Inline Functions
 ****************************************************************************/

#ifndef __ASSEMBLY__

static inline void flush_icache_line_indexed(unsigned long addr)
{
        __iflush_prologue
        cache_op(Index_Invalidate_I, addr);
        __iflush_epilogue
}

static inline void flush_dcache_line_indexed(unsigned long addr)
{
        __dflush_prologue
        cache_op(Index_Writeback_Inv_D, addr);
        __dflush_epilogue
}

static inline void flush_l2cache_line_indexed(unsigned long addr)
{
        cache_op(Index_Writeback_Inv_SD, addr);
}

static inline void flush_icache_line(unsigned long addr)
{
	__iflush_prologue
	cache_op(Hit_Invalidate_I, addr);
	__iflush_epilogue
}

static inline void flush_dcache_line(unsigned long addr)
{
	__dflush_prologue
	cache_op(Hit_Writeback_Inv_D, addr);
	__dflush_epilogue
}

static inline void invalidate_dcache_line(unsigned long addr)
{
	__dflush_prologue
	cache_op(Hit_Invalidate_D, addr);
	__dflush_epilogue
}

static inline void invalidate_l2cache_line(unsigned long addr)
{
	cache_op(Hit_Invalidate_SD, addr);
}

static inline void flush_l2cache_line(unsigned long addr)
{
	cache_op(Hit_Writeback_Inv_SD, addr);
}




#define cpu_dcache_size()               CONFIG_SYS_DCACHE_SIZE
#define cpu_dcache_ways()               CONFIG_SYS_DCACHE_WAYS
#define cpu_dcache_waysize()		(CONFIG_SYS_DCACHE_SIZE/CONFIG_SYS_DCACHE_WAYS)
#define cpu_dcache_line_size()          CONFIG_SYS_DCACHELINE_SIZE
#define cpu_icache_size()               CONFIG_SYS_ICACHE_SIZE
#define cpu_icache_ways()               CONFIG_SYS_ICACHE_WAYS
#define cpu_icache_waysize()		(CONFIG_SYS_ICACHE_SIZE/CONFIG_SYS_ICACHE_WAYS)
#define cpu_icache_line_size()          CONFIG_SYS_ICACHELINE_SIZE
#define cpu_l2cache_size()		CONFIG_SYS_L2CACHE_SIZE
#define cpu_l2cache_ways()		CONFIG_SYS_L2CACHE_WAYS
#define cpu_l2cache_waysize()		(CONFIG_SYS_L2CACHE_SIZE/CONFIG_SYS_L2CACHE_WAYS)
#define cpu_l2cache_line_size()		CONFIG_SYS_L2CACHELINE_SIZE


#define cache16_unroll32(base,op)                                       \
        __asm__ __volatile__(                                           \
        "       .set push                                       \n"     \
        "       .set noreorder                                  \n"     \
        "       .set mips32                                      \n"     \
        "       cache %1, 0x000(%0); cache %1, 0x010(%0)        \n"     \
        "       cache %1, 0x020(%0); cache %1, 0x030(%0)        \n"     \
        "       cache %1, 0x040(%0); cache %1, 0x050(%0)        \n"     \
        "       cache %1, 0x060(%0); cache %1, 0x070(%0)        \n"     \
        "       cache %1, 0x080(%0); cache %1, 0x090(%0)        \n"     \
        "       cache %1, 0x0a0(%0); cache %1, 0x0b0(%0)        \n"     \
        "       cache %1, 0x0c0(%0); cache %1, 0x0d0(%0)        \n"     \
        "       cache %1, 0x0e0(%0); cache %1, 0x0f0(%0)        \n"     \
        "       cache %1, 0x100(%0); cache %1, 0x110(%0)        \n"     \
        "       cache %1, 0x120(%0); cache %1, 0x130(%0)        \n"     \
        "       cache %1, 0x140(%0); cache %1, 0x150(%0)        \n"     \
        "       cache %1, 0x160(%0); cache %1, 0x170(%0)        \n"     \
        "       cache %1, 0x180(%0); cache %1, 0x190(%0)        \n"     \
        "       cache %1, 0x1a0(%0); cache %1, 0x1b0(%0)        \n"     \
        "       cache %1, 0x1c0(%0); cache %1, 0x1d0(%0)        \n"     \
        "       cache %1, 0x1e0(%0); cache %1, 0x1f0(%0)        \n"     \
        "       .set pop                                        \n"     \
                :                                                       \
                : "r" (base),                                           \
                  "i" (op));

#define cache32_unroll32(base,op)                                       \
        __asm__ __volatile__(                                           \
        "       .set push                                       \n"     \
        "       .set noreorder                                  \n"     \
        "       .set mips3                                      \n"     \
        "       cache %1, 0x000(%0); cache %1, 0x020(%0)        \n"     \
        "       cache %1, 0x040(%0); cache %1, 0x060(%0)        \n"     \
        "       cache %1, 0x080(%0); cache %1, 0x0a0(%0)        \n"     \
        "       cache %1, 0x0c0(%0); cache %1, 0x0e0(%0)        \n"     \
        "       cache %1, 0x100(%0); cache %1, 0x120(%0)        \n"     \
        "       cache %1, 0x140(%0); cache %1, 0x160(%0)        \n"     \
        "       cache %1, 0x180(%0); cache %1, 0x1a0(%0)        \n"     \
        "       cache %1, 0x1c0(%0); cache %1, 0x1e0(%0)        \n"     \
        "       cache %1, 0x200(%0); cache %1, 0x220(%0)        \n"     \
        "       cache %1, 0x240(%0); cache %1, 0x260(%0)        \n"     \
        "       cache %1, 0x280(%0); cache %1, 0x2a0(%0)        \n"     \
        "       cache %1, 0x2c0(%0); cache %1, 0x2e0(%0)        \n"     \
        "       cache %1, 0x300(%0); cache %1, 0x320(%0)        \n"     \
        "       cache %1, 0x340(%0); cache %1, 0x360(%0)        \n"     \
        "       cache %1, 0x380(%0); cache %1, 0x3a0(%0)        \n"     \
        "       cache %1, 0x3c0(%0); cache %1, 0x3e0(%0)        \n"     \
        "       .set pop                                        \n"     \
                :                                                       \
                : "r" (base),                                           \
                  "i" (op));

#define cache64_unroll32(base,op)                                       \
        __asm__ __volatile__(                                           \
        "       .set push                                       \n"     \
        "       .set noreorder                                  \n"     \
        "       .set mips3                                      \n"     \
        "       cache %1, 0x000(%0); cache %1, 0x040(%0)        \n"     \
        "       cache %1, 0x080(%0); cache %1, 0x0c0(%0)        \n"     \
        "       cache %1, 0x100(%0); cache %1, 0x140(%0)        \n"     \
        "       cache %1, 0x180(%0); cache %1, 0x1c0(%0)        \n"     \
        "       cache %1, 0x200(%0); cache %1, 0x240(%0)        \n"     \
        "       cache %1, 0x280(%0); cache %1, 0x2c0(%0)        \n"     \
        "       cache %1, 0x300(%0); cache %1, 0x340(%0)        \n"     \
        "       cache %1, 0x380(%0); cache %1, 0x3c0(%0)        \n"     \
        "       cache %1, 0x400(%0); cache %1, 0x440(%0)        \n"     \
        "       cache %1, 0x480(%0); cache %1, 0x4c0(%0)        \n"     \
        "       cache %1, 0x500(%0); cache %1, 0x540(%0)        \n"     \
        "       cache %1, 0x580(%0); cache %1, 0x5c0(%0)        \n"     \
        "       cache %1, 0x600(%0); cache %1, 0x640(%0)        \n"     \
        "       cache %1, 0x680(%0); cache %1, 0x6c0(%0)        \n"     \
        "       cache %1, 0x700(%0); cache %1, 0x740(%0)        \n"     \
        "       cache %1, 0x780(%0); cache %1, 0x7c0(%0)        \n"     \
        "       .set pop                                        \n"     \
                :                                                       \
                : "r" (base),                                           \
                  "i" (op));

#define cache128_unroll32(base,op)                                      \
        __asm__ __volatile__(                                           \
        "       .set push                                       \n"     \
        "       .set noreorder                                  \n"     \
        "       .set mips3                                      \n"     \
        "       cache %1, 0x000(%0); cache %1, 0x080(%0)        \n"     \
        "       cache %1, 0x100(%0); cache %1, 0x180(%0)        \n"     \
        "       cache %1, 0x200(%0); cache %1, 0x280(%0)        \n"     \
        "       cache %1, 0x300(%0); cache %1, 0x380(%0)        \n"     \
        "       cache %1, 0x400(%0); cache %1, 0x480(%0)        \n"     \
        "       cache %1, 0x500(%0); cache %1, 0x580(%0)        \n"     \
        "       cache %1, 0x600(%0); cache %1, 0x680(%0)        \n"     \
        "       cache %1, 0x700(%0); cache %1, 0x780(%0)        \n"     \
        "       cache %1, 0x800(%0); cache %1, 0x880(%0)        \n"     \
        "       cache %1, 0x900(%0); cache %1, 0x980(%0)        \n"     \
        "       cache %1, 0xa00(%0); cache %1, 0xa80(%0)        \n"     \
        "       cache %1, 0xb00(%0); cache %1, 0xb80(%0)        \n"     \
        "       cache %1, 0xc00(%0); cache %1, 0xc80(%0)        \n"     \
        "       cache %1, 0xd00(%0); cache %1, 0xd80(%0)        \n"     \
        "       cache %1, 0xe00(%0); cache %1, 0xe80(%0)        \n"     \
        "       cache %1, 0xf00(%0); cache %1, 0xf80(%0)        \n"     \
        "       .set pop                                        \n"     \
                :                                                       \
                : "r" (base),                                           \
                  "i" (op));


/*  build blast_xxx, blast_xxx_page, blast_xxx_page_indexed */
#define __BUILD_BLAST_CACHE(pfx, desc, indexop, hitop, lsize) \
static inline void blast_##pfx##cache##lsize(void)                      \
{                                                                       \
        unsigned long start = INDEX_BASE;                               \
        unsigned long end = start + cpu_##desc##_waysize(); 		\
        unsigned long ws_inc = cpu_##desc##_waysize();     		\
        unsigned long ws_end = cpu_##desc##_size();			\
        unsigned long ws, addr;                                         \
                                                                        \
        __##pfx##flush_prologue                                         \
                                                                        \
        for (ws = 0; ws < ws_end; ws += ws_inc)                         \
                for (addr = start; addr < end; addr += lsize * 32)      \
                        cache##lsize##_unroll32(addr|ws, indexop);      \
                                                                        \
        __##pfx##flush_epilogue                                         \
}                                                                       \
static inline void blast_##pfx##cache##lsize##_page(unsigned long page) \
{                                                                       \
        unsigned long start = page;                                     \
        unsigned long end = page + PAGE_SIZE;                           \
                                                                        \
        __##pfx##flush_prologue                                         \
                                                                        \
        do {                                                            \
                cache##lsize##_unroll32(start, hitop);                  \
                start += lsize * 32;                                    \
        } while (start < end);                                          \
                                                                        \
        __##pfx##flush_epilogue                                         \
}                                                                       \
                                                                        \
static inline void blast_##pfx##cache##lsize##_page_indexed(unsigned long page) \
{                                                                       \
       	unsigned long indexmask = cpu_##desc##_waysize() - 1;    	\
        unsigned long start = INDEX_BASE + (page & indexmask);          \
        unsigned long end = start + PAGE_SIZE;                          \
        unsigned long ws_inc = cpu_##desc##_waysize();     		\
        unsigned long ws_end = cpu_##desc##_size();			\
        unsigned long ws, addr;                                         \
                                                                        \
        __##pfx##flush_prologue                                         \
                                                                        \
        for (ws = 0; ws < ws_end; ws += ws_inc)                         \
                for (addr = start; addr < end; addr += lsize * 32)      \
                        cache##lsize##_unroll32(addr|ws, indexop);      \
                                                                        \
        __##pfx##flush_epilogue                                         \
}


__BUILD_BLAST_CACHE(d, dcache, Index_Writeback_Inv_D, Hit_Writeback_Inv_D, 16)
__BUILD_BLAST_CACHE(i, icache, Index_Invalidate_I, Hit_Invalidate_I, 16)
__BUILD_BLAST_CACHE(s, l2cache, Index_Writeback_Inv_SD, Hit_Writeback_Inv_SD, 16)
__BUILD_BLAST_CACHE(d, dcache, Index_Writeback_Inv_D, Hit_Writeback_Inv_D, 32)
__BUILD_BLAST_CACHE(i, icache, Index_Invalidate_I, Hit_Invalidate_I, 32)
__BUILD_BLAST_CACHE(s, l2cache, Index_Writeback_Inv_SD, Hit_Writeback_Inv_SD, 32)
__BUILD_BLAST_CACHE(d, dcache, Index_Writeback_Inv_D, Hit_Writeback_Inv_D, 64)
__BUILD_BLAST_CACHE(i, icache, Index_Invalidate_I, Hit_Invalidate_I, 64)
__BUILD_BLAST_CACHE(s, l2cache, Index_Writeback_Inv_SD, Hit_Writeback_Inv_SD, 64)
__BUILD_BLAST_CACHE(s, l2cache, Index_Writeback_Inv_SD, Hit_Writeback_Inv_SD, 128)

__BUILD_BLAST_CACHE(inv_d, dcache, Index_Writeback_Inv_D, Hit_Invalidate_D, 16)
__BUILD_BLAST_CACHE(inv_d, dcache, Index_Writeback_Inv_D, Hit_Invalidate_D, 32)
__BUILD_BLAST_CACHE(inv_s, l2cache, Index_Writeback_Inv_SD, Hit_Invalidate_SD, 16)
__BUILD_BLAST_CACHE(inv_s, l2cache, Index_Writeback_Inv_SD, Hit_Invalidate_SD, 32)
__BUILD_BLAST_CACHE(inv_s, l2cache, Index_Writeback_Inv_SD, Hit_Invalidate_SD, 64)
__BUILD_BLAST_CACHE(inv_s, l2cache, Index_Writeback_Inv_SD, Hit_Invalidate_SD, 128)






/*  build blast_xxx_range, protected_blast_xxx_range */
#define __BUILD_BLAST_CACHE_RANGE(pfx, desc, hitop, prot) \
static inline void prot##blast_##pfx##cache##_range(unsigned long start, \
                                                    unsigned long end)  \
{                                                                       \
        unsigned long lsize = cpu_##desc##_line_size();                 \
        unsigned long addr = start & ~(lsize - 1);                      \
        unsigned long aend = (end - 1) & ~(lsize - 1);                  \
                                                                        \
        __##pfx##flush_prologue                                         \
                                                                        \
        while (1) {                                                     \
                prot##cache_op(hitop, addr);                            \
                if (addr == aend)                                       \
                        break;                                          \
                addr += lsize;                                          \
        }                                                               \
                                                                        \
        __##pfx##flush_epilogue                                         \
}


__BUILD_BLAST_CACHE_RANGE(d, dcache, Hit_Writeback_Inv_D, )
__BUILD_BLAST_CACHE_RANGE(i, icache, Hit_Invalidate_I, )
__BUILD_BLAST_CACHE_RANGE(s, l2cache, Hit_Writeback_Inv_SD, )
/*  blast_inv_dcache_range */
__BUILD_BLAST_CACHE_RANGE(inv_d, dcache, Hit_Invalidate_D, )
__BUILD_BLAST_CACHE_RANGE(inv_s, l2cache, Hit_Invalidate_SD, )



#define flush_dcache_all()				        blast_dcache32()
#define flush_dcache_range(start, end) 			blast_dcache_range(start, end)
#define invalid_dcache_all()				    blast_inv_dcache32()
#define invalid_dcache_range(start, end)		blast_inv_dcache_range(start, end)

#define flush_icache_all()				        blast_icache32()
#define flush_icache_range(start, end)			blast_icache_range(start, end)

#define flush_l2cache_all()				        blast_scache64()
#define flush_l2cache_range(start, end)			blast_scache_range(start, end)
#define invalid_l2cache_all()				    blast_inv_scache64()
#define invalid_l2cache_range(start, end)		blast_inv_scache_range(start, end)


/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/


#undef EXTERN
#ifdef __cplusplus
}
#endif
#endif /* __ASSEMBLY__ */

#endif  /* __JZ_CACHE_H */
