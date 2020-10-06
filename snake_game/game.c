/*
 * 
 *   Name: game.c
 *   Contents: Contains all the main logic and main loop for the game
 *   Original Author Unknown.
 *   Most recent edit: 
 *   	Date: 10/5/2020
 *	Editor: Zachary E Graber (zegraber@iu.edu)
 *
 */

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "snake.h"
#include "food.h"
#include "game_window.h"
#include "key.h"
#include "game.h"

void generate_points(int *food_x, int *food_y, int width, int height, int x_offset, int y_offset){
    *food_x = rand() % width + x_offset;
    *food_y = rand() % height + y_offset;
}

void game(){
    enum State state = MENU; // Set the initial state
    static int x_max, y_max; //Max screen size variables
    static int x_offset, y_offset; // distance between the top left corner of your screen and the start of the board
    gamewindow_t *window; // Name of the board
    Snake *snake; // The snake
    Food *foods,*new_food; // List of foods (Not an array)
    int snakeDir; // The current direction the snake is moving.
    Snake *endOfTail; // The end of the snake's tail
    int endX; // X location of end of tail
    int endY; // Y location of end of tail
    int score = 0;

    const int height = 30; 
    const int width = 70;
    char ch;

    struct timespec timeret;
    timeret.tv_sec = 0;
    timeret.tv_nsec = 999999999/4;

    // Init all ncurses junk first, and only once.
    initscr();
    start_color();
    nodelay(stdscr, TRUE); //Dont wait for char
    noecho(); // Don't echo input chars
    getmaxyx(stdscr, y_max, x_max);
    keypad(stdscr, TRUE); // making keys work
    curs_set(0); // hide cursor
    timeout(100);


    while(state != EXIT){
        switch(state){
	case MENU:
	    clear();
	    // ASCII art made at http://patorjk.com/software/taag/#p=display&f=Doom&t=SNAKE
            mvprintw(0, 25, " _____ _   _   ___   _   __ _____");
	    mvprintw(1, 25, "/  ___| \\ | | / _ \\ | | / /|  ___|");
	    mvprintw(2, 25, "\\ `--.|  \\| |/ /_\\ \\| |/ / | |__");
	    mvprintw(3, 25, " `--. \\ . ` ||  _  ||    \\ |  __|");
	    mvprintw(4, 25, "/\\__/ / |\\  || | | || |\\  \\| |___");
	    mvprintw(5, 25, "\\____/\\_| \\_/\\_| |_/\\_| \\_/\\____/");

	    mvprintw(9, 5, " ____ ");
	    mvprintw(10, 5, "||S ||");
	    mvprintw(11, 5, "||__||    START GAME");
	    mvprintw(12, 5, "|/__\\|");

	    mvprintw(14, 5, " ____ ");
	    mvprintw(15, 5, "||Q ||");
	    mvprintw(16, 5, "||__||    QUIT");
	    mvprintw(17, 5, "|/__\\|");

	    mvprintw(9, 60, "Scoring");
	    mvprintw(10, 53, "---------------------");
	    mvprintw(11, 55, "+20 per good food: o");
	    mvprintw(12, 55, "-10 per bad food: x");

	    mvprintw(18, 38, " ____ "); 
	    mvprintw(19, 38, "||^ ||");
	    mvprintw(20, 38, "|||_||");
 	    mvprintw(21, 38, "|/__\\|");
	    mvprintw(23, 30, " ____    ____    ____"); 
	    mvprintw(24, 30, "||<-||  ||| ||  ||->||");
	    mvprintw(25, 30, "||__||  ||v_||  ||__||");
 	    mvprintw(26, 30,"|/__\\|  |/__\\|  |/__\\|");
	    mvprintw(28, 39, "MOVE");

	    ch = get_char();
	    switch(ch) {
		// Quit cases
		case 'q':
		    state = EXIT;
		    break;
		case 'Q':
		    state = EXIT;
		    break;

		// Start cases
		case 's':
		    state = INIT;
		    break;
		case 'S':
		    state = INIT;
		    break;
	    }

            break;

        case INIT:
	    // Setting height and width of the board
            x_offset = (x_max / 2) - (width / 2);
            y_offset = (y_max / 2) - (height / 2);
	    
	    // Init Game Window
	    window = init_GameWindow(x_offset, y_offset, width, height);
            draw_Gamewindow(window);

            // Init snake
            snake = init_snake(x_offset + (width / 2), y_offset + (height / 2));
            
            // Init foods
            int food_x, food_y, i;
            enum Type type;

            //Generate 20 foods
            generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
            type = (rand() > RAND_MAX/2) ? Increase : Decrease; // Randomly deciding type of food
            foods = create_food(food_x, food_y, type);
            for(i = 1; i < 20; i++){
                generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
                while (food_exists(foods,food_x, food_y))
                    generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
                type = (rand() > RAND_MAX/2) ? Increase : Decrease;
                new_food = create_food(food_x, food_y, type);
                add_new_food(foods, new_food);
            }
	    snakeDir = NOCHAR;
            state = ALIVE;
            break;

        case ALIVE:
	    endOfTail = get_end(snake);
	    endX = endOfTail->x;
	    endY = endOfTail->y;
	
	    if (score >= 100) {
	        timeret.tv_nsec = (999999999 / 4) / (1.5 * (score / 100));
	    }

            ch = get_char();
	    switch (ch) {
		// Movement cases. Prevent oppositional direction changes. 
		case UP:
		    if (snakeDir != DOWN) {
			snakeDir = UP;
		    }
		    break;
		case DOWN:
		    if (snakeDir != UP) {
			snakeDir = DOWN;
		    }
		    break;
		case LEFT:
		    if (snakeDir != RIGHT) {
			snakeDir = LEFT;
		    }
		    break;
		case RIGHT:
		    if (snakeDir != LEFT) {
			snakeDir = RIGHT;
		    }
		    break;

		// Exit cases
		case 'q':
		    state = EXIT;
		    break;
		case 'Q':
		    state = EXIT;
		    break;
	    }

	    // Move the snake (if the player has entered a direction yet
	    if (snakeDir != NOCHAR) {
		snake = move_snake(snake, snakeDir);
	    }

	    // Check to see if the snake's head is now in a food
	    if (food_exists(foods, snake->x, snake->y)) {
		switch (remove_eaten_food(foods, snake->x, snake->y)) {
		    case Increase:
			score += 20;
			(get_end(snake))->next = create_tail(endX, endY); // Add another tail on the end
			break;
		    case Decrease:
			score -= 10;
			snake = remove_tail(snake);
			break;
		}
		// spawn another food
		int food_x, food_y;
		generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
                while (food_exists(foods,food_x, food_y))
                    generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
                type = (rand() > RAND_MAX/2) ? Increase : Decrease;
                new_food = create_food(food_x, food_y, type);
                add_new_food(foods, new_food);
	    }

	    // Draw everything on the screen
            clear();
            mvprintw(20,20, "Key entered: %c", ch);
	    mvprintw(0, 0, "Score:  %d", score);
            draw_Gamewindow(window);
            draw_snake(snake);
            draw_food(foods);
            break;

        case DEAD:
            endwin();
            break;
        }
        refresh();
        nanosleep(&timeret, NULL);
    }
    clear();
    endwin();
}
