#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PID_ARR 512
#define FALSE 0
#define TRUE 1

static int ft_strlen(char *str)
{
	int i = 0;
	while (str[i]) i++;
	return i;
}

static void ft_error(char *msg, char *arg)
{
	write(2, "error: ", ft_strlen("error: "));
	write(2, msg, ft_strlen(msg));
	write(2, arg, ft_strlen(arg));
	write(2, "\n", 1);
}

static void ft_fatal()
{
	write(2, "error: fatal\n", ft_strlen("error: fatal\n"));
	exit(EXIT_FAILURE);
}

static void ft_cd(char **start, char **end)
{
	int i = 0;
	while (start[i] != *end) i++;
	if (i != 2)
		return ft_error("cd: bad arguments", "");

	if (chdir(start[1]) == -1)
		return ft_error("cd: cannot change directory to ", start[1]);
}

static void ft_child(char **start, char **end, char **envp, int input, int *fd_pipe, int is_pipe, int was_pipe)
{
	if (is_pipe) {
		if (dup2(fd_pipe[1], STDOUT_FILENO) == -1) ft_fatal();
		if (close(fd_pipe[0]) == -1) ft_fatal();
		if (close(fd_pipe[1]) == -1) ft_fatal();
	}

	if (was_pipe) {
		if (dup2(input, STDIN_FILENO) == -1) ft_fatal();
		if (close(input) == -1) ft_fatal();
	}

	*end = NULL;

	if (execve(start[0], start, envp) == -1)
		ft_error("cannot execute ", start[0]);

	exit(EXIT_FAILURE);
}

int main(int argc, char **argv, char **envp)
{
	if (argc <= 1) return EXIT_FAILURE;

	pid_t pid_arr[PID_ARR];
	for (int i = 0; i < PID_ARR; i++) pid_arr[i] = 0;

	int fd_pipe[2], input;
	int is_pipe = FALSE, was_pipe = FALSE;

	char **start = argv + 1;
	char **end = start;

	for (int i = 0; *end;)
	{
		while (*end && (strcmp(*end, "|") && strcmp(*end, ";"))) end++;

		is_pipe = (*end && **end == '|');

		input = (was_pipe ? fd_pipe[0] : STDIN_FILENO);

		if (is_pipe)
			if (pipe(fd_pipe)) ft_fatal();

		if (!strcmp(*start, "cd")) ft_cd(start, end);
		else if (start != end)
		{
			pid_t child = fork();
			if (child == -1) ft_fatal();
			if (!child)
				ft_child(start, end, envp, input, fd_pipe, is_pipe, was_pipe);

			if (!is_pipe) { if (waitpid(child, NULL, 0) == -1) ft_fatal(); }
			else {
				pid_arr[i] = child;
				i++;
			}
		}

		if (is_pipe) if (close(fd_pipe[1]) == -1) ft_fatal();
		if (was_pipe) if (close(input) == -1) ft_fatal();

		was_pipe = is_pipe;
		if (*end) end++;
		start = end;
	}

	for (int i = 0; pid_arr[i] && i < PID_ARR; i++)
		if (waitpid(pid_arr[i], NULL, 0) == -1) ft_fatal();

	printf("DEBUG LOOP\n");
	while (TRUE) ;

	return EXIT_SUCCESS;
}
