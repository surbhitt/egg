/* #include "font.cpp" */
#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>


using namespace std;

#define CANVAS_HEIGHT 40                        // rows
#define SPREAD_FACTOR 3
#define CANVAS_WIDTH CANVAS_HEIGHT              // cols
#define CANVAS_MID_X (int)(CANVAS_WIDTH / 2)
#define CANVAS_MID_Y (int)(CANVAS_HEIGHT / 2)

struct Pixel {
    uint32_t color;
    string glyph;
};

string block = "\u2588";
string no_block = "\u2800";
string trans_block = "\u2591";
uint32_t red = 0xff0000;
uint32_t green = 0x00ff00;
uint32_t blue = 0x0000ff;
uint32_t white = 0xffffff;
uint32_t black = 0x000000;

bool within_canvas(int rows, int cols) {
    return (rows < CANVAS_HEIGHT && cols < CANVAS_WIDTH);
}

bool within_canvas(int idx) { return idx < CANVAS_HEIGHT * CANVAS_WIDTH; }

int euclidean_distance(int i, int j, int x, int y) {
    return sqrt((i - x) * (i - x) + (j - y) * (j - y));
}

void fill_canvas_bg(Pixel *pixels, uint32_t bg_color) {
    for (int i = 0; i < CANVAS_HEIGHT * CANVAS_WIDTH; i++) {
        pixels[i].color = bg_color;
        pixels[i].glyph = block;
    }
}

void fill_square(Pixel *pixels, int side, int row_n, int col_n, uint32_t color,
                 string glyph) {
    for (int i = 0; i < side * side; i++) {
        int row = i / side + row_n, // row
            col = i % side + col_n, // column
            idx = row * CANVAS_WIDTH + col;
        if (col < CANVAS_WIDTH && within_canvas(idx)) {
            pixels[idx].color = color;
            pixels[idx].glyph = glyph;
        }
    }
}

void fill_circle(Pixel *pixels, int radius, int center_col, int center_row,
                 uint32_t color, string glyph) {
    // Imagine a sq box of side radius
    // The circle will lie inside this box
    // For each point in the box check
    // if the distance from center is <= radius
    int box_top_row = max(0, center_row - radius),
        box_bot_row = min(center_row + radius, CANVAS_HEIGHT - 1),
        box_left_col = max(0, center_col - radius),
        box_right_col = min(center_col + radius, CANVAS_WIDTH - 1);
    int dist_from_c = 0;
    for (int r = box_top_row; r <= box_bot_row; r++) {
        for (int c = box_left_col; c <= box_right_col; c++) {
            dist_from_c = euclidean_distance(r, c, center_row, center_col);
            if (dist_from_c <= radius) {
                pixels[r * CANVAS_WIDTH + c].color = color;
                pixels[r * CANVAS_WIDTH + c].glyph = glyph;
            }
        }
    }
    // TODO: Implement anti aliasing
}

Pixel make_pixel(string glyph, uint32_t color) {
    Pixel p;
    p.glyph = glyph;
    p.color = color;
    return p;
}

void convert_to_pixel(Pixel sub_pixels[6][5], char glyph[6][5]) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 5; j++) {
            sub_pixels[i][j] = glyph[i][j] ? make_pixel(block, white)
                                           : make_pixel(no_block, white);
        }
    }
}

string construct_foreg_color(uint32_t color) {
    string r = to_string((color >> 16) & 0xFF);
    string g = to_string((color >> 8) & 0xFF);
    string b = to_string(color & 0xFF);

    // format of the code ESC[38;2;{r};{g};{b}m
    string color_str = "\033[38;2;";
    color_str += r + ';' + g + ';' + b + 'm';
    return color_str;
}

void draw_canvas(Pixel *pixels) {
    cout << "\033[H";
    for (int i = 0; i < CANVAS_HEIGHT * CANVAS_WIDTH; i++) {
        if (i % CANVAS_WIDTH == 0)
            cout << '\n';
        string fcolor = construct_foreg_color(pixels[i].color);
        for (int i = 0; i < SPREAD_FACTOR; i++)
            cout << fcolor << pixels[i].glyph;
    }
}

void cursor_to_home() {
    // take cursor to the home position
    cout << "\033[H";
}

void clear_till_eos() {
    // clear screen from curr pos to end of screen
    cout << "\033[0J";
}

void hide_cursor() {
    // hide the cursor from the screen
    cout << "\033[?25l";
}

void show_cursor() {
    // make the hidden cursor appear
    cout << "\033[?25h";
}

void clear_screen() {
    cursor_to_home();
    clear_till_eos();
    hide_cursor();
}

/**
 Linux (POSIX) implementation of kbhit().
 Morgan McGuire, morgan@cs.brown.edu
 */
int kbhit() {
    static const int STDIN = 0;
    // persists the variable across function calls
    static bool initialized = false;

    if (! initialized) {
        termios term;
        tcgetattr(STDIN, &term);
        // set to non canonical mode
        // to turn off line buffering
        term.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}

// get keyboard input
char get_kb_in() {
    char ch = '\0';
    if (kbhit()) {
        ch = getchar();
    }
    return ch;
}

// check for q key press
bool end_gl() {
    // end game loop
    char ch = get_kb_in();
    if (ch == 'q' || ch == 'Q') return true;
    return false;
}

