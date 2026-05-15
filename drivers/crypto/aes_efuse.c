#include <aes_efuse.h>
#include <linux/compiler.h>
#include <common.h>

void aes_efuse_init(uint32_t *aesiv, ModeType mode, KEY_SIZE key_size)
{
	__cpm_start_aes();

    int i;
	/* Clear AES done*/
    aes_writel(aes_readl(ASSR),ASSR);
	/* enable AES dma done interrupt */
    aes_writel(0x2,ASINTM);
	/* Clear the IV and KEYS; enable AES module */
    aes_writel(3 << 28 | 1<<10 | 1 << 0,ASCR);
	/* Enable AES DMA*/
    aes_writel(1<<8 |aes_readl(ASCR),ASCR);
	/* Use user key */
    aes_writel(1<<19 |aes_readl(ASCR),ASCR);
	if(key_size == KEY_SIZE_256) {
	/* Use key 256bit ,use user key also 256bit */
    aes_writel(2<<6 | 3<<16 |aes_readl(ASCR),ASCR);
	}else{
	/* Use key 128bit ,use user key also 128bit */
    aes_writel(1<<16 |aes_readl(ASCR),ASCR);
	}

    if((mode == ENC_ECB ) || (mode == DEC_ECB)){
        if(mode == DEC_ECB){
			/*big-endian input, little-endian output*/
			aes_writel(1<<13 |aes_readl(ASCR),ASCR);
            /* Enable Decrypts, ECB mode */
            aes_writel(1<<4 |aes_readl(ASCR),ASCR);
        }else{
			/*little-endian input, big-endian output*/
			aes_writel(1<<12 |aes_readl(ASCR),ASCR);
		}
    }else{
		if(mode == ENC_CBC){
			/*little-endian input, big-endian output*/
			aes_writel(1<<12 |aes_readl(ASCR),ASCR);
			/* Enable Encrypts, CBC mode */
			aes_writel(1<<5 | aes_readl(ASCR),ASCR);
		}else{
			/*big-endian input, little-endian output*/
			aes_writel(1<<13 |aes_readl(ASCR),ASCR);
            /* Enable Decrypts, CBC mode */
            aes_writel(1<<5 | 1<<4 | aes_readl(ASCR),ASCR);
        }
        for(i = 0; i < 4; i++)
            aes_writel(aesiv[i],ASIV);

        /* write IV init */
        aes_writel(aes_readl(ASCR) | 1<<1,ASCR);
    }

    /* write KEYS start */
    aes_writel(aes_readl(ASCR) | 1<<2,ASCR);

}

void aes_efuse_start(uint32_t data_in, uint32_t len,uint32_t data_out)
{
	/* config aes dma */
    aes_writel(data_in,ASSA);
    aes_writel((len / 4),ASSTC);
    aes_writel(data_out,ASDA);
    aes_writel((len / 4),ASDTC);
    aes_writel((len / 16),ASBNUM);
	/* start aes dma */
    aes_writel(aes_readl(ASCR) | 1<<9,ASCR);
	/* wait aes dma done */
	while (!(aes_readl(ASSR) & (1 << 1)));
    aes_writel(aes_readl(ASSR),ASSR);
	printf("aes dma done\n");
}

uint32_t get_aes_header_length_and_iv(uint32_t img_phys_addr, uint32_t *iv_out)
{
	void *virt_addr;
	uint8_t *iv_ptr;
	uint8_t header[20];
	uint32_t encrypted_size;

	/* Map header memory (need 20 bytes) */
	virt_addr = map_sysmem(img_phys_addr, 20);
	if (!virt_addr) {
		printf("AES: Failed to map address 0x%08x\n", img_phys_addr);
		return 0;
	}

	/* Read 20-byte header (4-byte size header + 16-byte IV) */
	memcpy(header, virt_addr, 20);

	/* Parse big-endian encrypted data size (first 4 bytes) */
	encrypted_size = (header[0] << 24) | (header[1] << 16) |
		(header[2] << 8) | header[3];

	/* Copy IV (next 16 bytes) as uint32_t array if provided */
	if (iv_out) {
		/* Direct memory copy - ensure proper alignment */
		memcpy(iv_out, header + 4, 16);

		iv_ptr = header + 4;
		iv_out[0] = (iv_ptr[0] << 24) | (iv_ptr[1] << 16) | (iv_ptr[2] << 8) | iv_ptr[3];
		iv_out[1] = (iv_ptr[4] << 24) | (iv_ptr[5] << 16) | (iv_ptr[6] << 8) | iv_ptr[7];
		iv_out[2] = (iv_ptr[8] << 24) | (iv_ptr[9] << 16) | (iv_ptr[10] << 8) | iv_ptr[11];
		iv_out[3] = (iv_ptr[12] << 24) | (iv_ptr[13] << 16) | (iv_ptr[14] << 8) | iv_ptr[15];
	}

	/* Validate: must be multiple of 16 (AES block size) */
	if (encrypted_size % 16 != 0) {
		printf("AES: Header length %u is not 16-byte aligned\n", encrypted_size);
		return 0;
	}

	return encrypted_size;
}
