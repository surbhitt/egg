/*
 * Generate a grid using backtracking
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cstdint>
#include <cstdlib>
#include <stack>
#include <vector>
#include "../egg.cpp"
#include "gif.h"

#define CELL_SIZE 2
#define OFFSET 1
const int working_grid_h = (CANVAS_HEIGHT - 1) / CELL_SIZE;
const int working_grid_w = (CANVAS_WIDTH - 2) / CELL_SIZE;

//  GIF related
const char* fileName = "maze.gif";
int delay = 100;
GifWriter g;

bool bound_check(int x, int y) {
    if (x < working_grid_h && x >= 0 && y < working_grid_w && y >= 0)
        return true;
    return false;
}

void explore_next(pair<int, int> maze[working_grid_h][working_grid_w],
                  bool vis[working_grid_h][working_grid_w],
                  stack<pair<int, int>>& st) {
    if (st.empty())
        return;
    auto [curx, cury] = st.top();
    vis[curx][cury] = true;

    //                       N        S       E       W
    pair<int, int> dir[] = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}};
    vector<int> neighbours;

    for (int i = 0; i < (int)size(dir); i++) {
        int neighbour_x = curx + dir[i].first,
            neighbour_y = cury + dir[i].second;
        if (bound_check(neighbour_x, neighbour_y) &&
            !vis[neighbour_x][neighbour_y]) {
            neighbours.push_back(i);
        }
    }

    if (neighbours.size()) {
        int rand_n = neighbours[rand() % neighbours.size()];
        int neighbour_x = curx + dir[rand_n].first,
            neighbour_y = cury + dir[rand_n].second;
        st.push({neighbour_x, neighbour_y});
        switch (rand_n) {
            case 0:
                // north
                maze[neighbour_x][neighbour_y].first = 1;
                break;
            case 1:
                // south
                maze[curx][cury].first = 1;
                break;
            case 2:
                // east
                maze[curx][cury].second = 1;
                break;
            case 3:
                // west
                maze[neighbour_x][neighbour_y].second = 1;
                break;
        }
    } else {
        if (!st.empty())
            st.pop();
    }

    return;
}

int main() {
    srand(time(NULL));

    // REQUIRED DATA STRUCTURES =============================================
    // south east
    pair<int, int> maze[working_grid_h][working_grid_w]{};
    bool vis[working_grid_h][working_grid_w]{};
    stack<pair<int, int>> st;
    st.push({0, 0});
    vis[0][0] = true;

    Pixel pixels[CANVAS_HEIGHT * CANVAS_WIDTH];

    GifBegin(&g, fileName, CANVAS_WIDTH, CANVAS_HEIGHT, delay);
    clear_screen();

    // GAME LOOP ============================================================
    while (!end_gl()) {
        cursor_to_home();
        fill_canvas_bg(pixels, red);

        // takes the current stack top and explores the next neighbour randomly
        if (!st.empty())
            explore_next(maze, vis, st);

        // render
        for (int i = 0; i < working_grid_h; i++) {
            for (int j = 0; j < working_grid_w; j++) {
                int idx = (CELL_SIZE * i + OFFSET) * CANVAS_WIDTH +
                          CELL_SIZE * j + OFFSET;
                if (vis[i][j]) {
                    pixels[idx].color = white;
                    if (maze[i][j].first) {
                        pixels[idx + CANVAS_WIDTH].color = white;
                    }
                    if (maze[i][j].second) {
                        pixels[idx + 1].color = white;
                    }
                }
            }
        }

        // start
        pixels[(CELL_SIZE * 0 + OFFSET) * CANVAS_WIDTH + CELL_SIZE * 0 + OFFSET]
            .color = blue;
        // end
        pixels[(CELL_SIZE * (working_grid_h - 1) + OFFSET) * CANVAS_WIDTH +
               CELL_SIZE * (working_grid_w - 1) + OFFSET]
            .color = green;

        // the south and east of the cell act as the overall maze boundary
        // so remove the explicit maze boundary to mantain symmetry
        // remove bottom boundary
        for (int i = 0; i < CANVAS_WIDTH; i++) {
            pixels[(CANVAS_HEIGHT - 1) * CANVAS_WIDTH + i].color = black;
        }
        // remove right boundary
        for (int i = 0; i < CANVAS_HEIGHT; i++) {
            pixels[(CANVAS_WIDTH - 1) + i * CANVAS_WIDTH].color = black;
        }

        draw_canvas(pixels);
        usleep(1000 * 1000 / 100);

        uint8_t* frame = new uint8_t[CANVAS_HEIGHT * CANVAS_WIDTH];
        for (int i = 0; i < CANVAS_HEIGHT; i++) {
            for (int j = 0; j < CANVAS_WIDTH; j++) {
                if (pixels[i * CANVAS_WIDTH + j].color == black) {
                    frame[i * CANVAS_WIDTH + j] = 0;
                } else if (pixels[i * CANVAS_WIDTH + j].color == white) {
                    frame[i * CANVAS_WIDTH + j] = 1;
                } else if (pixels[i * CANVAS_WIDTH + j].color == red) {
                    frame[i * CANVAS_WIDTH + j] = 2;
                } else if (pixels[i * CANVAS_WIDTH + j].color == blue) {
                    frame[i * CANVAS_WIDTH + j] = 4;
                } else if (pixels[i * CANVAS_WIDTH + j].color == green) {
                    frame[i * CANVAS_WIDTH + j] = 3;
                }
            }
        }
        GifWriteFrame(&g, frame, CANVAS_WIDTH, CANVAS_HEIGHT, delay);
        delete[] frame;
    }
    show_cursor();

    GifEnd(&g);
    return 0;
}
