void game();
int num_saves();
void top_10(int *top10Ptr);
void generate_points(int *food_x, int *food_y, int width, int height, int x_offset, int y_offset);
enum State{INIT, ALIVE, PAUSED, DEAD, EXIT, MENU, DIFFICULTY_SELECT};
enum Difficulty{EASY, NORMAL, HARD};
