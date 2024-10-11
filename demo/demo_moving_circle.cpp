#include "../egg.cpp"

int main() {
    Pixel *pixels;
    pixels = new Pixel[CANVAS_HEIGHT * CANVAS_WIDTH];
    int i = -CANVAS_MID_X;
    clear_screen();
    while (true) {
        cursor_to_home();
        fill_canvas_bg(pixels, green);
        fill_circle(pixels, 15, i, CANVAS_MID_Y, red, block);
        /* fill_square(pixels, 20, 2, i % CANVAS_WIDTH, red, block); */
        draw_canvas(pixels);
        i++;
        if (i > CANVAS_WIDTH + CANVAS_MID_X)
            i = -CANVAS_MID_X;
        usleep(1000 * 1000 / 60);
    }
    /* cursor_to_home(); */
    /* fill_canvas_bg(pixels, green); */
    /* fill_circle(pixels, 15, CANVAS_MID_X, CANVAS_MID_Y, red, block); */
    /* draw_canvas(pixels); */
    show_cursor();
}
