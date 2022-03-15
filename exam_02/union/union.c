#include <unistd.h>

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
	index = 1;
	i = 0;
	while (argv[index] && argv[index][i])
	{
		if (table[argv[(int)index][i]] == 0)
		{
			write(1, &argv[index][i], 1);
			table[(int)argv[index][i]] = 1;
		}
		i++;
		if (argv[index][i] == '\0')
		{
			i = 0;
			index++;
		}
	}
	write(1, "\n", 1);
	return (0);
}

