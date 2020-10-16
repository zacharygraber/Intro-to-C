/*food.h -------
*
* Filename: food.h
* Description:
* Author: Manisha Suresh Kumar
* Maintainer:
* Created: Sat Sep 12 13:21:55 2020
* Last-Updated: October 11 2020
*	  By: Zachary E Graber (zegraber@iu.edu)
*
*/

/* Commentary:
*
*
*
*/

/* Change log:
*
*   + Added prototype for new function type_of_food()
*   ~ Changed parameters for remove_eaten_food()
*/

/*Copyright (c) 2016 The Trustees of Indiana University and
* Indiana University Research and Technology Corporation.
*
* All rights reserved.
*
* Additional copyrights may follow
*/

#include <ncurses.h>

//Two types of food
//Either Increase or Decrease length of snake
enum Type {Increase, Decrease}; 

//Structure to hold properties of food
struct Food {
  int x;
  int y;
  char type;
  struct Food* next;
};

typedef struct Food Food; 

//Function prototypes 
void add_new_food(Food* foods, Food* new_food);
bool food_exists(Food* foods, int x, int y);
Food* create_food(int x, int y, enum Type type);
enum Type type_of_food(Food* foods, int x, int y);
enum Type remove_eaten_food(Food** foodsPtr, int x, int y);
void draw_food(Food *food);
