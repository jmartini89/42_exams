CC=gcc $(DEBUG) $(ERROR) $(BUFFER)
DEBUG=-g -fsanitize=address
ERROR=-Wall -Werror -Wextra
BUFFER=-D BUFFER_SIZE=$(SIZE)
SIZE=20

NAME=gnl.out
SRC= main.c get_next_line.c

all: $(NAME)

$(NAME):
	$(CC) $(SRC) -o $@

clean:
	rm -f $(NAME)

re: clean all
