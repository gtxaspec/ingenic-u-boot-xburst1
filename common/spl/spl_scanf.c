#include <common.h>
#include <stdarg.h>
#include <linux/string.h>
#include <linux/ctype.h>
#define USHRT_MAX     ((u16)(~0U))
#define SHRT_MAX     ((s16)(USHRT_MAX>>1))

/**
* simple_strtoll - convert a string to a signed long long
* @cp: The start of the string
* @endp: A pointer to the end of the parsed string will be placed here
* @base: The number base to use
*/
long long simple_strtoll(const char *cp, char **endp, unsigned int base)
{
     if (*cp == '-')
          return -simple_strtoull(cp + 1, endp, base);

     return simple_strtoull(cp, endp, base);
}

/* Convert a character to lower case */
__inline__ static char
_tolower (char c)
{
  if ((c >= 'A') && (c <= 'Z'))
    {
      c = (c - 'A') + 'a';
    }
  return c;
}

#define is_digit(c)	((c) >= '0' && (c) <= '9')

static int skip_atoi(const char **s)
{
	int i = 0;

	while (is_digit(**s))
		i = i * 10 + *((*s)++) - '0';

	return i;
}

/**
* vsscanf - Unformat a buffer into a list of arguments
* @buf:     input buffer
* @fmt:     format of buffer
* @args:     arguments
*/
int vsscanf(const char *buf, const char *fmt, va_list args)
{
     const char *str = buf;
     char *next;
     char digit;
     int num = 0;
     u8 qualifier;
     u8 base;
     s16 field_width;
     char is_sign;

     while (*fmt && *str) {
          /* skip any white space in format */
          /* white space in format matchs any amount of
          * white space, including none, in the input.
          */
          if (isspace(*fmt)) {
               fmt = skip_spaces(++fmt);
               str = skip_spaces(str);
          }

          /* anything that is not a conversion must match exactly */
          if (*fmt != '%' && *fmt) {
               if (*fmt++ != *str++)
                    break;
               continue;
          }

          if (!*fmt)
               break;
          ++fmt;

          /* skip this conversion.
          * advance both strings to next white space
          */
          if (*fmt == '*') {
               while (!isspace(*fmt) && *fmt != '%' && *fmt)
                    fmt++;
               while (!isspace(*str) && *str)
                    str++;
               continue;
          }

          /* get field width */
          field_width = -1;
          if (isdigit(*fmt))
               field_width = skip_atoi(&fmt);

          /* get conversion qualifier */
          qualifier = -1;
          if (*fmt == 'h' || _tolower(*fmt) == 'l' ||
              _tolower(*fmt) == 'z') {
               qualifier = *fmt++;
               if (unlikely(qualifier == *fmt)) {
                    if (qualifier == 'h') {
                         qualifier = 'H';
                         fmt++;
                    } else if (qualifier == 'l') {
                         qualifier = 'L';
                         fmt++;
                    }
               }
          }

          if (!*fmt || !*str)
               break;

          base = 10;
          is_sign = 0;

          switch (*fmt++) {
          case 'c':
          {
               char *s = (char *)va_arg(args, char*);
               if (field_width == -1)
                    field_width = 1;
               do {
                    *s++ = *str++;
               } while (--field_width > 0 && *str);
               num++;
          }
          continue;
          case 's':
          {
               char *s = (char *)va_arg(args, char *);
               if (field_width == -1)
                    field_width = SHRT_MAX;
               /* first, skip leading white space in buffer */
               str = skip_spaces(str);

               /* now copy until next white space */
               while (*str && !isspace(*str) && field_width--)
                    *s++ = *str++;
               *s = '\0';
               num++;
          }
          continue;
          case 'n':
               /* return number of characters read so far */
          {
               int *i = (int *)va_arg(args, int*);
               *i = str - buf;
          }
          continue;
          case 'o':
               base = 8;
               break;
          case 'x':
          case 'X':
               base = 16;
               break;
          case 'i':
               base = 0;
          case 'd':
               is_sign = 1;
          case 'u':
               break;
          case '%':
               /* looking for '%' in str */
               if (*str++ != '%')
                    return num;
               continue;
          default:
               /* invalid format; stop here */
               return num;
          }

          /* have some sort of integer conversion.
          * first, skip white space in buffer.
          */
          str = skip_spaces(str);

          digit = *str;
          if (is_sign && digit == '-')
               digit = *(str + 1);

          if (!digit
              || (base == 16 && !isxdigit(digit))
              || (base == 10 && !isdigit(digit))
              || (base == 8 && (!isdigit(digit) || digit > '7'))
              || (base == 0 && !isdigit(digit)))
               break;

          switch (qualifier) {
          case 'H':     /* that's 'hh' in format */
               if (is_sign) {
                    signed char *s = (signed char *)va_arg(args, signed char *);
                    *s = (signed char)simple_strtol(str, &next, base);
               } else {
                    unsigned char *s = (unsigned char *)va_arg(args, unsigned char *);
                    *s = (unsigned char)simple_strtoul(str, &next, base);
               }
               break;
          case 'h':
               if (is_sign) {
                    short *s = (short *)va_arg(args, short *);
                    *s = (short)simple_strtol(str, &next, base);
               } else {
                    unsigned short *s = (unsigned short *)va_arg(args, unsigned short *);
                    *s = (unsigned short)simple_strtoul(str, &next, base);
               }
               break;
          case 'l':
               if (is_sign) {
                    long *l = (long *)va_arg(args, long *);
                    *l = simple_strtol(str, &next, base);
               } else {
                    unsigned long *l = (unsigned long *)va_arg(args, unsigned long *);
                    *l = simple_strtoul(str, &next, base);
               }
               break;
          case 'L':
               if (is_sign) {
                    long long *l = (long long *)va_arg(args, long long *);
                    *l = simple_strtoll(str, &next, base);
               } else {
                    unsigned long long *l = (unsigned long long *)va_arg(args, unsigned long long *);
                    *l = simple_strtoull(str, &next, base);
               }
               break;
          case 'Z':
          case 'z':
          {
               size_t *s = (size_t *)va_arg(args, size_t *);
               *s = (size_t)simple_strtoul(str, &next, base);
          }
          break;
          default:
               if (is_sign) {
                    int *i = (int *)va_arg(args, int *);
                    *i = (int)simple_strtol(str, &next, base);
               } else {
                    unsigned int *i = (unsigned int *)va_arg(args, unsigned int*);
                    *i = (unsigned int)simple_strtoul(str, &next, base);
               }
               break;
          }
          num++;

          if (!next)
               break;
          str = next;
     }

     /*
     * Now we've come all the way through so either the input string or the
     * format ended. In the former case, there can be a %n at the current
     * position in the format that needs to be filled.
     */
     if (*fmt == '%' && *(fmt + 1) == 'n') {
          int *p = (int *)va_arg(args, int *);
          *p = str - buf;
     }

     return num;
}

/**
* sscanf - Unformat a buffer into a list of arguments
* @buf:     input buffer
* @fmt:     formatting of buffer
* @...:     resulting arguments
*/
int sscanf(const char *buf, const char *fmt, ...)
{
     va_list args;
     int i;

     va_start(args, fmt);
     i = vsscanf(buf, fmt, args);
     va_end(args);

     return i;
}

int scanf(const char* fmt, ...)
{
	int i = 0;
	unsigned char c;
	va_list args;
	char buffer[128];

	while(i < sizeof(buffer))
	{
		c = serial_getc();
		serial_putc(c);
		if ((0x0d == c) || (0x0a == c))
		{
			buffer[i] = '\0';
			break;
		}
		else
		{
			buffer[i++] = c;
		}
	}

	va_start(args,fmt);
	i = vsscanf(buffer,fmt,args);
	va_end(args);

	serial_putc('r');
	serial_putc('n');
	return i;
}
