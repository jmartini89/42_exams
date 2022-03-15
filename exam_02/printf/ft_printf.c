#include "header.h"
#include <stdarg.h>
#include <unistd.h>

void
	ft_putchar(char c, int *cnt)
{
	write(1, &c, 1);
	*cnt = *cnt + 1;
}

void
	ft_putstr(char *str, int *cnt)
{
	while (*str)
	{
		ft_putchar(*str, cnt);
		str++;
	}
}

void
	ft_conv_str(char *str, int *cnt)
{

	if (str)
		ft_putstr(str, cnt);
	else
		ft_putstr("(null)", cnt);
}

void
	ft_putnbr_dec(long int n, int *cnt)
{
	if (n < 0)
	{
		ft_putchar('-', cnt);
		n = n * -1;
	}
	if (n > 10)
		ft_putnbr_dec(n / 10, cnt);
	ft_putchar((n % 10) + 48, cnt);
}

void
	ft_putnbr_hex(unsigned long int n, int *cnt)
{
	if (n >= 16)
		ft_putnbr_hex(n / 16, cnt);
	if ((n % 16) > 9)
		ft_putchar((n % 16)+ 87, cnt);
	else
		ft_putchar((n % 16) + 48, cnt);
}
	
void
	ft_engine(const char **str, va_list args, int *cnt)
{
	if (**str == 's')
	{
		ft_conv_str(va_arg(args, char *), cnt);
		*str = *str + 1;
	}
	if (**str == 'd')
	{
		ft_putnbr_dec(va_arg(args, int), cnt);
		*str = *str + 1;
	}
	if (**str == 'x')
	{
		ft_putnbr_hex(va_arg(args, unsigned int), cnt);
		*str = *str + 1;
	}
}
	
int
	ft_printf(const char *str, ...)
{
	va_list	args;
	int		cnt;

	cnt = 0;
	va_start(args, str);
	while (*str)
	{
		if (*str == '%')
		{
			str++;
			ft_engine(&str, args, &cnt);
		}
		else
		{
			ft_putchar(*str, &cnt);
			str++;
		}
	}
	va_end(args);
	return (cnt);
}

