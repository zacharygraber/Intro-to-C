/*food.c -------
*
* Filename: food.c
* Description:
* Author: Manisha Suresh Kumar
* Maintainer:
* Created: Sat Sep 12 13:16:12 2020
* Last-Updated: October 16 2020
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
*  + added colors to food drawing
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
#define CYAN 5
#define RED 2

//Create new food
Food* create_food(int x, int y, enum Type type){
    Food* new_food = malloc(sizeof(Food));
     
    new_food->x = x;
    new_food->y = y;
    if (type == Increase){
        new_food->type = (rand() > RAND_MAX / 2) ? 'O' : '+';
    }
    else if(type == Decrease){
        new_food->type = (rand() > RAND_MAX / 2) ? 'X' : '-';
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

// A function dedicated to returning the type of food at a point x, y
enum Type type_of_food(Food* foods, int x, int y) {
	char typeOfFood;
	Food *temp = foods;
	while (!(temp->x == x && temp->y == y)) {
		temp = temp->next;
	}
	typeOfFood = temp->type;
	return (typeOfFood == 'O' || typeOfFood == '+') ? Increase : Decrease;	
}

// Given a food at x, y, removes it from foods and returns the type of food that it was
enum Type remove_eaten_food(Food** foodsPtr, int x, int y){
    // Get type of food at the position
    char typeOfFood = type_of_food(*foodsPtr, x, y);

    // Remove the eaten food.
    // Since the list of foods is just a singly-linked list, we use a standard node deletion algorithm.
    Food* temp = *foodsPtr;
    
    // First, consider the edge case where the food in question is the first node in the list.
    if (temp->x == x && temp->y == y) {
	Food* firstNode = *foodsPtr; // Make a temporary ptr to store the first node's address
	*foodsPtr = (*foodsPtr)->next; // Set the first node to the next one
	free(firstNode);
    }
    else {
	// Search for the node BEFORE the one we're trying to delete.
	while (!((temp->next)->x == x && (temp->next)->y == y)) {
		temp = temp->next;
	}
	// Now that we have the node before...
	Food* deletedNode = temp->next; // Save a ptr to store the address of the node we're gonna delete
	temp->next = deletedNode->next; // Bypass the node we're deleting
	free(deletedNode); // Free it
    }
	
    return typeOfFood;
}

// Display all the food
// Good food is blue, bad food is yellow
void draw_food (Food *foods)
{   Food* temp = foods;
    while(temp) {
	switch (temp->type) {
	    case '+':
	    case 'O':
		attron(COLOR_PAIR(CYAN));
		break;
	    case 'X':
	    case '-':
		attron(COLOR_PAIR(RED));
		break;
	}
        mvprintw(temp->y, temp->x, "%c", temp->type);
	switch (temp->type) {
	    case '+':
	    case 'O':
		attroff(COLOR_PAIR(CYAN));
		break;
	    case 'X':
	    case '-':
		attroff(COLOR_PAIR(RED));
		break;
	}
        temp = temp->next;
    }
}
