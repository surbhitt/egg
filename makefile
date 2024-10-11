CC=g++
CFLAGS=-Wall -Wextra -Werror -g

all:
	$(CC) $(CFLAGS) -o ./demo/dmc ./demo/demo_moving_circle.cpp
	./demo/dmc


