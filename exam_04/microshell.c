// MICROSHELL
// to my dear friend Pierre:
// yes, syntax was pretty weird, I'm redeeming myself now
// It's been a while but I'll try my best to comment some passages for clarity
// Love you Pierre, godspeed

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// pray that nobody will ever spawn more than 512 processes
#define PID_ARR 512
// booleans, because they're comfy
#define FALSE 0
#define TRUE 1

static int ft_strlen(char *str) {
	int i = 0;
	while (str[i])
		i++;
	return i;
}

// automated error output
static void ft_error(char *msg, char *arg) {
	write(2, "error: ", ft_strlen("error: "));
	write(2, msg, ft_strlen(msg));
	write(2, arg, ft_strlen(arg));
	write(2, "\n", 1);
}

// a syscall failed: panic & exit
static void ft_fatal() {
	write(2, "error: fatal\n", ft_strlen("error: fatal\n"));
	exit(EXIT_FAILURE);
}

static void ft_cd(char **start, char **end) {
	int i = 0;
	while (start[i] != *end)
		i++;
	if (i != 2)
		return ft_error("cd: bad arguments", "");

	if (chdir(start[1]) == -1)
		return ft_error("cd: cannot change directory to ", start[1]);
}

// child process routine
static void ft_child(char **start, char **end, char **envp, int input, int *fd_pipe, int is_pipe, int was_pipe) {
	// MAN fork:
	// The child inherits COPIES of the parent's set of open file descriptors

	// overwrite STDOUT with current PIPE_WRITE
	// close unused FDs
	if (is_pipe) {
		if (dup2(fd_pipe[1], STDOUT_FILENO) == -1)
			ft_fatal();
		if (close(fd_pipe[0]) == -1)
			ft_fatal();
		if (close(fd_pipe[1]) == -1)
			ft_fatal();
	}

	// overwrite STDIN with old PIPE_READ
	// close unused FD
	if (was_pipe) {
		if (dup2(input, STDIN_FILENO) == -1)
			ft_fatal();
		if (close(input) == -1)
			ft_fatal();
	}

	// substring terminator
	*end = NULL;

	if (execve(start[0], start, envp) == -1)
		ft_error("cannot execute ", start[0]);

	// since execve overwrites child memspace
	// if child execution arrives here, execve failed
	// preventing child from executing calling code
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv, char **envp) {
	if (argc <= 1)
		return EXIT_FAILURE;

	pid_t pid_arr[PID_ARR];
	for (int i = 0; i < PID_ARR; i++)
		pid_arr[i] = 0;

	int fd_pipe[2], input;
	int is_pipe = FALSE, was_pipe = FALSE;

	char **start = argv + 1;
	char **end = start;

	for (int i = 0; *end;) {
		// reading argv[1] string and saving its end address
		// end address will be set to NULL to compose substring
		while (*end && (strcmp(*end, "|") && strcmp(*end, ";")))
			end++;

		// boolean check: is the iteration a pipe?
		is_pipe = (*end && **end == '|');

		// if previous iteration was a pipe: save its PIPE_READ as current input
		// otherwise reset to STDIN
		input = (was_pipe ? fd_pipe[0] : STDIN_FILENO);

		if (is_pipe) {
			if (pipe(fd_pipe))
				ft_fatal();
		}

		if (!strcmp(*start, "cd"))
			ft_cd(start, end);
		else if (start != end) {
			pid_t child = fork();
			if (child == -1)
				ft_fatal();
			if (!child)
				ft_child(start, end, envp, input, fd_pipe, is_pipe, was_pipe);

			// not a pipe: wait child exit
			// a pipe: concurrency needed, save child's PID for final wait
			if (!is_pipe) {
				if (waitpid(child, NULL, 0) == -1)
					ft_fatal();
			}
			else {
				pid_arr[i] = child;
				i++;
			}
		}

		// close new (PIPE_WRITE) & old (PIPE_READ) FDs
		if (is_pipe) {
			if (close(fd_pipe[1]) == -1)
				ft_fatal();
		}
		if (was_pipe) {
			if (close(input) == -1)
				ft_fatal();
		}

		// update pipe status
		was_pipe = is_pipe;
		// keep reading from end + 1
		if (*end)
			end++;
		start = end;
	}

	// wait for concurrent childrens
	for (int i = 0; pid_arr[i] && i < PID_ARR; i++) {
		if (waitpid(pid_arr[i], NULL, 0) == -1)
			ft_fatal();
	}

	// debug loop to be used with LSOF
	// printf("DEBUG LOOP\n");
	// while (TRUE) ;

	return EXIT_SUCCESS;
}
