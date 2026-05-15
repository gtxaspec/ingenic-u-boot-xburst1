/*
 * arch/mips/include/asm/jz_cache.h
 *
 * Copyright (c) 2016 Ingenic Semiconductor Co.,Ltd
 * 	qipengzhen <aric.pzqi@ingenic.com>
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
#ifndef __DMA_DEFAULT_H__
#define __DMA_DEFAULT_H__

#include <asm/jz_cache.h>


enum dma_data_direction {
	DMA_BIDIRECTIONAL = 0,
	DMA_TO_DEVICE = 1,
	DMA_FROM_DEVICE = 2,
	DMA_NONE = 3,
};

void dma_cache_sync(void *dev, void *vaddr, size_t size,
		enum dma_data_direction direction);


#endif

