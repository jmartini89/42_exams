#include "header.h"
#include <stdio.h>
#define STR "test %s%s%s %d %x\n", "engine", buf, "", 42, 42
	
int
	main(void)
{
	int		ret;
	char	*buf;
	int	a;

	buf = NULL;
	printf("%d\n", ft_printf(STR));
	printf("%d\n", printf(STR));

	return (0);
}

