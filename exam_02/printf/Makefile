NAME=ft_printf.out
CC=gcc
DEBUG=-g -fsanitize=address

SRC=main.c ft_printf.c

all: $(NAME)

$(NAME):
	$(CC) $(DEBUG) $(SRC) -o $@

clean:
	rm -f $(NAME)

re: clean all
