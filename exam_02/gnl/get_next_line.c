#include "get_next_line.h"
#include <stdio.h>

static void
	ft_bzero(char *p, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		p[i] = '\0';
		i++;
	}
}

static char *
	ft_calloc(size_t count, size_t size)
{
	char	*heap;

	heap = malloc(count * size);
	ft_bzero(heap, count * size);
	return (heap);
}

static int
	ft_strlen(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

static char *
	ft_strjoin(char *a, char *b)
{
	char	*str;
	int	len;
	int	i;
	int	j;

	len = ft_strlen(a) + ft_strlen(b) + 1;
	str = ft_calloc(len, sizeof(*str));
	
	i = 0;
	while (a[i])
	{
		str[i] = a[i];
		i++;
	}
	
	j= 0;
	while (b[j])
	{
		str[i] = b[j];
		i++;
		j++;
	}
	
	return (str);
}

static int
	ft_chain(char *tmp, char **line)
{
	char	*swap;

	swap = *line;
	*line = ft_strjoin(swap, tmp);
	if (*line == NULL)
		return (0);
	free (swap);
	free (tmp);
	return (1);
}

static int
	ft_engine(char *buffer, char **line)
{
	static int	buf_ind;
	int		i;
	char		*tmp;

	tmp = ft_calloc(BUFFER_SIZE + 1, sizeof(*tmp));
	if (tmp == NULL)
		return (-1);

	i = 0;
	while (buf_ind < BUFFER_SIZE)
	{
		if (tmp[i] == '\n')
		{
			if (ft_chain(tmp, line) == 0)
				return (-1);
			buf_ind++;
			return (1);
		}
		tmp[i++] = buffer[buf_ind++];
	}
	
	if (ft_chain(tmp, line) == 0)
		return (-1);
	
	buf_ind = 0;
	return (0);
}

char *
	get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	char		*line;
	int		ret;
	int		status;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);

	line = ft_calloc(BUFFER_SIZE + 1, sizeof(*line));
	if (line == NULL)
		return (NULL);

	status = 0;
	ret = 1;
	while (ret)
	{
		status = ft_engine(buffer, &line);
		if (status == -1)
			return (NULL);
		if (status == 0)
		{
			ft_bzero(buffer, BUFFER_SIZE);
			ret = read(fd, buffer, BUFFER_SIZE);
			if (ret == -1)
				return (NULL);
		}
		if (status == 1)
			return (line);
	}

	if (*line)
		return (line);

	free(line);
	return (NULL);
}

