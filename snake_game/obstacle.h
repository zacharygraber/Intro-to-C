/*obstacle.h -------
*
* Filename: obstacle.h
* Description:
* Author: Zachary E Graber
* Maintainer:
* Created: October 11 2020
* Last-Updated: October 16 2020
*	  By: Zachary E Graber (zegraber@iu.edu)
*
*/

/* Change log:
*
*   + created
*   + added method free_obstacles(Obstacle *obsts)
*
*/

#include <ncurses.h>

//Structure to hold properties of and obstacle
struct Obstacle {
  int x; // Top left corner 
  int y;
  int size; // Obstacles are square. This is the sidelength.
  struct Obstacle* next;
};

typedef struct Obstacle Obstacle; 

// Function prototypes 
void add_new_obstacle(Obstacle* obstacles, Obstacle* new_obstacle);
bool obstacle_exists(Obstacle* obstacles, int x, int y);
Obstacle* create_obstacle(int x, int y, int size);
void draw_obstacles(Obstacle *obstacles);
void free_obstacles(Obstacle *obsts);
