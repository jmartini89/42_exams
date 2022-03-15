CC=gcc

SRC_MICRO=./micro_paint/micro_marta.c
SRC_MINI=./mini_paint/mini.c
OBJ_MICRO=$(SRC_MICRO:.c=.o)
OBJ_MINI=$(SRC_MINI:.c=.o)

all: micro.out mini.out

micro.out: $(OBJ_MICRO)
	gcc $(OBJ_MICRO) -lm -o micro.out

mini.out: $(OBJ_MINI)
	gcc $(OBJ_MINI) -lm -o mini.out

clean:
	rm -f micro.out mini.out $(OBJ_MICRO) $(OBJ_MINI)

re: clean all