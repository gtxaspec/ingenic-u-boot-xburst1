/* #define CONFIG_DWC_DEBUG 1 */
#ifdef CONFIG_DWC_DEBUG
#define dwc_debug(fmt, args...)  printf(fmt, ##args)
#define sys_ddrc(fmt, addr)      dwc_debug("%s addr(0x%x) 0x%x\n", fmt, addr, ddrc_readl(addr))
#define PRINT_DDRP(var)          PRINTF_DDRP(__LINE__, #var, READ_INNOPHY_REG2(var))

static void PRINTF_DDRP(unsigned int line, const char* name, unsigned int value)
{
	dwc_debug("%d\t%s\r\t\t\t\t\t0x%x\n", line, name, value);
}
#else
#define dwc_debug(fmt, args...)
#define sys_ddrc(fmt, addr)
#define PRINT_DDRP(var)
#endif
