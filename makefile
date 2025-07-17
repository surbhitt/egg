CC=g++
CFLAGS=-Wall -Wextra -Werror -g

all: demo_game_of_life

demo_moving_circle:
	$(CC) $(CFLAGS) -o ./demo/dmc.out ./demo/demo_moving_circle.cpp
	./demo/dmc.out

demo_game_of_life:
	$(CC) $(CFLAGS) -o ./demo/dgol.out ./demo/demo_game_of_life.cpp
	./demo/dgol.out

demo_snake:
	$(CC) $(CFLAGS) -o ./demo/dsnk.out ./demo/demo_snake.cpp
	./demo/dsnk.out

demo_maze_gen:
	$(CC) $(CFLAGS) -o ./demo/dmzgn.out ./demo/demo_maze_gen.cpp
	./demo/dmzgn.out

