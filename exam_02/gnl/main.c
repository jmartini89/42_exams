#include "get_next_line.h"
#include <stdio.h>

int
	main(void)
{
	int	fd;
	char	*ret;

	ret = "file.txt";
	fd = open(ret, O_RDONLY);
	if (fd == -1)
		return (1);

	while (ret)
	{
		ret = get_next_line(fd);
		if (ret)
			printf("%s\n", ret);
		free (ret);
	}
	
	return (0);
}

