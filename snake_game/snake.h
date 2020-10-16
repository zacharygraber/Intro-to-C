/* snake.h -------
 *
 * Filename: snake.h
 * Description:
 * Author: Rishabh Agrawal
 * Maintainer:
 * Created: Sun Sep 13 21:01:02 2020
 * Last-Updated: October 5 2020
 *           By: Zachary Graber (zegraber@iu.edu)
 *
 */

/* Commentary:
 *
 *
 *
 */

/* Change log:
 *
 * + added prototype for function get_end
 * ~ Updated parameters for draw_snake
 * + added prototype for function free_snake
 *
 */

/* Copyright (c) 2016 The Trustees of Indiana University and
 * Indiana University Research and Technology Corporation.
 *
 * All rights reserved.
 *
 * Additional copyrights may follow
 */


#include <stdbool.h>

struct Snake {
  int x;
  int y;
  int speed;
  char color[3];
  char symbol;
  struct Snake* next;
};

typedef struct Snake Snake;

Snake* init_snake(int x, int y);
Snake* create_tail(int x, int y);
Snake* move_snake(Snake* snake, int direction);
void draw_snake(Snake* snake, int color);
bool eat_itself(Snake* snake);
Snake* remove_tail(Snake* snake);
int len(Snake* snake);
Snake* get_end(Snake* snake);
void free_snake(Snake* snake);
