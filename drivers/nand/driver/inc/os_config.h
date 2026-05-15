#include <config.h>

#ifdef CONFIG_JZ4780
#define CONFIG_SOC_4780
#elif defined(CONFIG_JZ4775)
#define CONFIG_SOC_4775
#elif defined(CONFIG_M200)
#define CONFIG_SOC_M200
#elif defined(CONFIG_T10)
#define CONFIG_SOC_T10
#elif defined(CONFIG_T5)
#define CONFIG_SOC_T5
#endif
