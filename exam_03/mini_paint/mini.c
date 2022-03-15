#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ERR1 "Error: argument\n"
#define ERR2 "Error: Operation file corrupted\n"

#define OUT 0
#define IN 1
#define PERI 2

int
ft_strlen(char *str) {
	int i = 0;
	while (str[i]) i++;
	return i;
}

int
ft_perror(char *str) {
	write(1, str, ft_strlen(str));
	return 1;
}

int
ft_checkpos(float x, float y, float id_x, float id_y, float radius) {
	float distance = sqrtf(powf(x - id_x, 2.0) + powf(y - id_y, 2.0));
	if (distance <= radius) {
		if ((radius - distance) < 1.0) return PERI;
		return IN;
	}
	return OUT;
}

int
main(int argc, char **argv) {
	FILE	*file;
	char	*paper;
	int		read, pos, x, y;

	int		b_width, b_height;
	char	background;

	float	id_x, id_y, radius;
	char	id, color;

	if (argc != 2) return ft_perror(ERR1);

	if (!(file = fopen(argv[1], "r"))
		|| (fscanf(file, "%d %d %c\n", &b_width, &b_height, &background) != 3)
		|| !(b_width > 0 && b_width <= 300 && b_height > 0 && b_height <= 300)
		|| !(paper = malloc(sizeof(*paper) * b_width * b_height)))
		return ft_perror(ERR2);

	memset(paper, background, b_width * b_height);

	while ((read = fscanf(file, "%c %f %f %f %c\n", &id, &id_x, &id_y, &radius, &color)) == 5) {
		if (!(radius > 0) || !(id == 'C' || id == 'c')) break;

		y = -1;
		while (++y < b_height) {
			x = -1;
			while (++x < b_width) {
				pos = ft_checkpos((float)x, (float)y, id_x, id_y, radius);
				if (pos == PERI || (pos == IN && id == 'C'))
					paper[x + y * b_width] = color;
			}
		}
	}

	if (read != EOF) {
		free(paper);
		fclose(file);
		return ft_perror(ERR2);
	}

	y = -1;
	while (++y < b_height) {
		write(1, paper + y * b_width, b_width);
		write(1, "\n", 1);
	}

	free(paper);
	fclose(file);
	return 0;
}
