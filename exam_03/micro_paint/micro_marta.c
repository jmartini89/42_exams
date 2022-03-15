#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define ERR1 "Error: argument\n"
#define ERR2 "Error: Operation file corrupted\n"

#define OUT 0
#define IN 1
#define PERI 2

int ft_strlen(char *str)
{
	int	idx = 0;
	while (str[idx])
		idx++;
	return (idx);
}

int	ft_perror(char *str)
{
	write(1, str, ft_strlen(str));
	return (1);
}

int	check_pos(float x, float y, float id_x, float id_y, float width, float height)
{
	if (x < id_x || x > id_x + width || y < id_y || y > id_y + height)
		return (OUT);
	else if (x - id_x < 1.0 || (id_x + width) - x < 1.0
			|| y - id_y < 1.0 || (id_y + height) - y < 1.0)
		return (PERI);
	return (IN);
}

int	main(int argc, char **argv)
{
	FILE	*file;
	char	*paper;
	int		read, pos;
	int		x, y;

	int		b_width, b_height;
	char	background;

	char	id, color;
	float	id_x, id_y, width, height;

	if (argc != 2)
		return (ft_perror(ERR1));
	if (!(file = fopen(argv[1], "r"))
		|| (fscanf(file, "%d %d %c\n", &b_width, &b_height, &background) != 3)
		|| (!(b_width > 0 && b_width <= 300 && b_height > 0 && b_height <= 300))
		|| (!(paper = (char *)malloc(sizeof(char) * (b_width * b_height)))))
		return (ft_perror(ERR2));
	memset(paper, background, b_width * b_height);
	while ((read = fscanf(file, "%c %f %f %f %f %c\n", &id, &id_x, &id_y, &width, &height, &color)) == 6)
	{
		if (!(width > 0 && height > 0) || !(id == 'R' || id == 'r'))
			break;
		y = -1;
		while (++y < b_height)
		{
			x = -1;
			while (++x < b_width)
			{
				pos = check_pos((float)x, (float)y, id_x, id_y, width, height);
				if (pos == PERI || (pos == IN && id == 'R'))
					paper[y * b_width + x] = color;
			}
		}
	}
	if (read != EOF)
	{
		free(paper);
		fclose(file);
		return (ft_perror(ERR2));
	}
	y = -1;
	while (++y < b_height)
	{
		write(1, paper + y * b_width, b_width);
		write(1, "\n", 1);
	}
	free(paper);
	fclose(file);
	return (0);
}
