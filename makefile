CC=g++
CFLAGS=-Wall -Wextra -Werror -g

all: demo_game_of_life

demo_moving_circle:
	$(CC) $(CFLAGS) -o ./demo/dmc.out ./demo/demo_moving_circle.cpp
	./demo/dmc.out

demo_game_of_life:
	$(CC) $(CFLAGS) -o ./demo/dgol.out ./demo/demo_game_of_life.cpp
	./demo/dgol.out

