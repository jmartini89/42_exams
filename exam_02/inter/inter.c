#include <unistd.h>
#define TRUE 1
#define FALSE 0

int
	main(int argc, char **argv)
{
	int	table[127] = {0};
	int	index;
	int	i;

	if (argc != 3)
	{
		write(1, "\n", 1);
		return (1);
	}
	index = 2;
	i = 0;
	while (index && argv[index][i])
	{
		if (index == 1 && table[(int)argv[index][i]] == TRUE)
		{
			table[(int)argv[index][i]] = FALSE;
			write(1, &argv[index][i], 1);
		}
		if (index == 2 && table[(int)argv[index][i]] == FALSE)
			table[(int)argv[index][i]] = TRUE;
		i++;
		if (argv[index][i] == '\0')
		{
			i = 0;
			index--;
		}
	}
	write(1, "\n", 1);
	return (0);
}

