/*
 * Implementation of the game of life
 * https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cstdlib>
#include <vector>
#include "../egg.cpp"

bool bound_check(int x, int y) {
    if (x >= 0 && x < CANVAS_HEIGHT && y >= 0 && y < CANVAS_WIDTH)
        return true;
    return false;
}

void calculate_next_state(int grid[CANVAS_HEIGHT][CANVAS_WIDTH]) {
    vector<pair<int, int>> n_coords = {
        {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1},
    };
    int x, y, n_alive;

    int next_grid[CANVAS_HEIGHT][CANVAS_WIDTH];

    for (int i = 0; i < CANVAS_HEIGHT; i++) {
        for (int j = 0; j < CANVAS_WIDTH; j++) {
            n_alive = 0;
            for (auto& n_coord: n_coords) {
                x = i+n_coord.first;
                y = j+n_coord.second;
                if (bound_check(x, y)) {
                    if (grid[x][y]) n_alive++;
                }
            }
            if (n_alive > 3 || n_alive < 2) next_grid[i][j] = 0;
            else if (n_alive == 3) next_grid[i][j] = 1;
            else next_grid[i][j] = grid[i][j];
        }
    }
    
    // copy the next state
    for (int i = 0; i < CANVAS_HEIGHT; i++) {
        for (int j = 0; j < CANVAS_WIDTH; j++) {
            grid[i][j] = next_grid[i][j];
        }
    }
}

int main() {
    srand(time(NULL));
    int grid[CANVAS_HEIGHT][CANVAS_WIDTH];

    for (int i = 0; i < CANVAS_HEIGHT; i++) {
        for (int j = 0; j < CANVAS_WIDTH; j++) {
            grid[i][j] = rand() % 13 == 1 ? 1 : 0;
        }
    }

    Pixel* pixels;
    pixels = new Pixel[CANVAS_HEIGHT * CANVAS_WIDTH];

    clear_screen();
    while (true) {
        cursor_to_home();
        fill_canvas_bg(pixels, green);

        for (int i = 0; i < CANVAS_HEIGHT; i++) {
            for (int j = 0; j < CANVAS_WIDTH; j++) {
                if (grid[i][j])
                    pixels[i * CANVAS_WIDTH + j].color = red;
            }
        }

        draw_canvas(pixels);
        usleep(1000 * 1000 / 5);

        // calculate next state
        calculate_next_state(grid);
    }
    show_cursor();
    return 0;
}
