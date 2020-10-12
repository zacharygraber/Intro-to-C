/*obstacle.c -------
*
* Filename: obstacle.c
* Description:
* Author: Zachary E Graber
* Maintainer:
* Created: October 11 2020
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
*  + created
*
*/

#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>
#include "obstacle.h"


// Create new obstacle
Obstacle* create_obstacle(int x, int y, int size){
    Obstacle* new_obst = malloc(sizeof(Obstacle));
     
    new_obst->x = x;
    new_obst->y = y;
    new_obst->size = size;
    new_obst->next = NULL;

    return new_obst;
}

//Check if there is an obstacle at these coordinates
bool obstacle_exists(Obstacle *obstacles, int x, int y){
    Obstacle* temp = obstacles;
    int i, j;
    while (temp) {
        for (i = 0; i < temp->size; i++) {
	    for (j = 0; j < temp->size; j++) {
		if (temp->x + i == x && temp->y + j == y)
		    return true;
	    }
	}
        temp = temp->next;
    }
    return false;
}

//Add new obstacle to end of obstacles list
void add_new_obstacle(Obstacle* obstacles, Obstacle* new_obstacle){
    Obstacle* temp = obstacles;
    while(temp->next) {
        temp = temp->next;
    }
    temp->next = new_obstacle;
}

// Display all the obstacles
void draw_obstacles(Obstacle *obstacles){
    Obstacle* temp = obstacles;
    int i, j;
    while(temp) {
	for (i = 0; i < temp->size; i++) {
	    for (j = 0; j < temp->size; j++) {
		mvprintw(temp->y + j, temp->x + i, "!");
	    }
	}
        temp = temp->next;
    }
}
