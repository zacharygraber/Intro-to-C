#include "game_window.h"

int num_saves();
void top_10(int *top10Ptr);
void game();
void generate_points(int *food_x, int *food_y, int width, int height, int x_offset, int y_offset);
enum State{INIT, ALIVE, PAUSED, DEAD, EXIT, MENU, DIFFICULTY_SELECT, TOP_10, LOAD};
enum Difficulty{EASY, NORMAL, HARD};
int display_lives(gamewindow_t *window, int lives_remaining);
