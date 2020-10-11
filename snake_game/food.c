/*food.c -------
*
* Filename: food.c
* Description:
* Author: Manisha Suresh Kumar
* Maintainer:
* Created: Sat Sep 12 13:16:12 2020
* Last-Updated: October 5 2020
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
*  + added logic for remove_eaten_food function
*
*/

/*Copyright (c) 2016 The Trustees of Indiana University and
* Indiana University Research and Technology Corporation.
*
* All rights reserved.
*
* Additional copyrights may follow
*/

#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>
#include "food.h"


//Create new food
Food* create_food(int x, int y, enum Type type){
    Food* new_food = malloc(sizeof(Food));
     
    new_food->x = x;
    new_food->y = y;
    if (type == Increase){
        new_food->type = 'O';
    }
    else if(type == Decrease){
        new_food->type = 'X';
    }
    new_food->next = NULL;

    return new_food;
}

//Check if food exists at coordinates
bool food_exists(Food* foods, int x, int y){
    Food* temp = foods;
    while(temp){
        if(temp->x == x && temp->y == y)
            return true;
        temp = temp->next;
    }
    return false;
}

//Add new food to end of food list
void add_new_food(Food* foods, Food* new_food){
    Food* temp = foods;
    while(temp->next) {
        temp = temp->next;
    }
    temp->next = new_food;
}

// Given a food at x, y, removes it from foods and returns the type of the food
enum Type remove_eaten_food(Food* foods, int x, int y){
    char typeOfFood;
    Food *temp = foods;
    while (temp) {
	if (temp->x == x && temp->y == y) {
	    typeOfFood = temp->type;
	    break;
	}
	else {
	    temp = temp->next;
	}
    }

    // Now that we have the food in question, we delete it as if it were a node in a singly-linked list.
    if (temp == foods) {
	printf("HERE\n");
	foods = temp->next; // Consider the edge case where the food in question is the first one
	return typeOfFood == 'O' ? Increase : Decrease;
    }
    else {
	// Traverse the list until we reach the node before temp
	Food *oneBeforeTemp = foods;
	while (oneBeforeTemp->next != temp) {
	    oneBeforeTemp = oneBeforeTemp->next;
	}
	// Once we've found it, replace its 'next' attribute with temp's 'next'
    	oneBeforeTemp->next = temp->next;
    }

    free(temp);
    return typeOfFood == 'O' ? Increase : Decrease;
}

// Display all the food
void draw_food (Food *foods)
{   Food* temp = foods;
    while(temp) {
        mvprintw(temp->y, temp->x, "%c", temp->type);
        temp = temp->next;
    }
}
