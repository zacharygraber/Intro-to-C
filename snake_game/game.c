/*
 * 
 *   Name: game.c
 *   Contents: Contains all the main logic and main loop for the game
 *   Original Author Unknown.
 *   Most recent edit: 
 *   	Date: 10/16/2020
 *	Editor: Zachary E Graber (zegraber@iu.edu)
 *
 */

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include "snake.h"
#include "food.h"
#include "key.h"
#include "game.h"
#include "obstacle.h"

#define GREEN 1
#define RED 2
#define BLUE 3
#define YELLOW 4
#define CYAN 5
#define MAGENTA 6

void generate_points(int *food_x, int *food_y, int width, int height, int x_offset, int y_offset){
    *food_x = rand() % width + x_offset;
    *food_y = rand() % height + y_offset;
}

void game(){
    enum State state = MENU; // Set the initial state
    enum Difficulty difficulty = NORMAL; // Difficulty of the game.
    static int x_max, y_max; //Max screen size variables
    static int x_offset, y_offset; // distance between the top left corner of your screen and the start of the board
    gamewindow_t *window = NULL; // Name of the board
    Snake *snake = NULL, *enemy_snake = NULL; // The snake and the AI one
    Snake *tail, *enemy_tail;
    Food *foods = NULL, *new_food; // List of foods (Not an array)
    Obstacle *obstacles = NULL, *new_obstacle; // List of obstacles.
    int snakeDir; // The current direction the snake is moving.
    int enemySnakeDir = RIGHT; // Direction the enemy snake
    int endX; // X location of end of tail
    int endY; // Y location of end of tail
    int score;
    int scoreThisLevel;
    int boardIncreases = 0;
    int numSaves = num_saves();
    bool checkedHighScores;
    int pause_menu_selection = 1;
    int lives_remaining;
    int extra_sleep_flag = 0;

    const int height = 30; 
    const int width = 70;
    char ch;

    struct timespec timeret;
    timeret.tv_sec = 0;
    timeret.tv_nsec = 999999999/4;

    // Init all ncurses junk first, and only once.
    initscr();
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_CYAN, COLOR_BLACK);
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
    nodelay(stdscr, TRUE); //Dont wait for char
    noecho(); // Don't echo input chars
    getmaxyx(stdscr, y_max, x_max);
    keypad(stdscr, TRUE); // making keys work
    curs_set(0); // hide cursor
    timeout(100);


    while(state != EXIT){

	if (extra_sleep_flag > 0) {
		sleep(extra_sleep_flag);
		extra_sleep_flag = 0;
	}

        switch(state){
	case MENU:
	    clear();
	    // ASCII art made at http://patorjk.com/software/taag/#p=display&f=Doom&t=SNAKE
	    attron(COLOR_PAIR(GREEN));
            mvprintw(0, 25, " _____ _   _   ___   _   __ _____");
	    mvprintw(1, 25, "/  ___| \\ | | / _ \\ | | / /|  ___|");
	    mvprintw(2, 25, "\\ `--.|  \\| |/ /_\\ \\| |/ / | |__");
	    mvprintw(3, 25, " `--. \\ . ` ||  _  ||    \\ |  __|");
	    mvprintw(4, 25, "/\\__/ / |\\  || | | || |\\  \\| |___");
	    mvprintw(5, 25, "\\____/\\_| \\_/\\_| |_/\\_| \\_/\\____/");
	    attroff(COLOR_PAIR(GREEN));

	    mvprintw(9, 5, " ____ ");
	    mvprintw(10, 5, "||S ||");
	    mvprintw(11, 5, "||__||    START GAME");
	    mvprintw(12, 5, "|/__\\|");

	    mvprintw(14, 5, " ____ ");
	    mvprintw(15, 5, "||Q ||");
	    mvprintw(16, 5, "||__||    QUIT");
	    mvprintw(17, 5, "|/__\\|");

	    mvprintw(9, 62, "Scoring");
	    mvprintw(10, 53, "-------------------------");
	    mvprintw(11, 55, "+20 per good food: o/+");
	    mvprintw(12, 55, "-10 per bad food: x/-");

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
		    state = DIFFICULTY_SELECT;
		    break;
		case 'S':
		    state = DIFFICULTY_SELECT;
		    break;
	    }
            break;
	case DIFFICULTY_SELECT:
	    clear();
	    attron(COLOR_PAIR(GREEN));
	    mvprintw(0, 20, "______ _  __  __ _            _ _");
	    mvprintw(1, 20, "|  _  (_)/ _|/ _(_)          | | |");
	    mvprintw(2, 20, "| | | |_| |_| |_ _  ___ _   _| | |_ _   _ ");
	    mvprintw(3, 20, "| | | | |  _|  _| |/ __| | | | | __| | | |");
	    mvprintw(4, 20, "| |/ /| | | | | | | (__| |_| | | |_| |_| |");
	    mvprintw(5, 20, "|___/ |_|_| |_| |_|\\___|\\__,_|_|\\__|\\__, |");
	    mvprintw(6, 20, "                                     __/ |");
	    mvprintw(7, 20, "                                    |___/ ");
	    attroff(COLOR_PAIR(GREEN));


	    attron(COLOR_PAIR(YELLOW));
	    mvprintw(12, 38, "NORMAL");
	    attroff(COLOR_PAIR(YELLOW));
	    mvprintw(12, 20, "EASY");
	    attron(COLOR_PAIR(RED));
	    mvprintw(12, 57, "HARD");
	    attroff(COLOR_PAIR(RED));
	    
	    int selectionBoxX;
	    switch (difficulty) {
		case EASY:
		    selectionBoxX = 17;
		    break;
		case NORMAL:
		    selectionBoxX = 36;
		    break;
		case HARD:
		    selectionBoxX = 54;
		    break;
	    }

	    attron(COLOR_PAIR(BLUE));
	    mvprintw(10, selectionBoxX, " --------");
	    int i;
	    for (i = 11; i < 14; i++) {
		mvprintw(i, selectionBoxX, "|");
		mvprintw(i, selectionBoxX + 9, "|");
	    }
	    mvprintw(14, selectionBoxX, " --------");
	    attroff(COLOR_PAIR(BLUE));
	
	    mvprintw(18, 21, "SELECT");
	    mvprintw(20, 17, " ____    ____"); 
	    mvprintw(21, 17, "||<-||  ||->||");
	    mvprintw(22, 17, "||__||  ||__||");
 	    mvprintw(23,17, "|/__\\|  |/__\\|");
	    
	    mvprintw(18, 51, "CONFIRM");
	    mvprintw(20, 48, " ____________"); 
	    mvprintw(21, 48, "||   ENTER  ||");
	    mvprintw(22, 48, "||__________||");
 	    mvprintw(23, 48, "|/__________\\|");

	    ch = get_char();
	    switch (ch) {
		case LEFT:
		    if (difficulty == HARD)
			difficulty = NORMAL;
		    else if (difficulty == NORMAL)
	
		difficulty = EASY;
		    break;
		case RIGHT:
		    if (difficulty == EASY)
			difficulty = NORMAL;
		    else if (difficulty == NORMAL)
			difficulty = HARD;
		    break;
		case '\n':
		    state = INIT;
		    break;
	    }
	    break;

	case PAUSED:
	    clear();
	    int x, y; // Middle of the game board
	    x = window->upper_left_x + (window->width / 2);
	    y = window->upper_left_y + (window->height / 2);

	    draw_Gamewindow(window);

	    attron(COLOR_PAIR(GREEN));
	    mvprintw(y-10, x-15, "______                        _ ");
	    mvprintw(y-9, x-15, "| ___ \\                      | |");
	    mvprintw(y-8, x-15, "| |_/ /_ _ _   _ ___  ___  __| |");
	    mvprintw(y-7, x-15, "|  __/ _` | | | / __|/ _ \\/ _` |");
	    mvprintw(y-6, x-15, "| | | (_| | |_| \\__ \\  __/ (_| |");
	    mvprintw(y-5, x-15, "\\_|  \\__,_|\\__,_|___/\\___|\\__,_|");
	    attroff(COLOR_PAIR(GREEN));

	    mvprintw(y-1,x-24," --------    ----------    ----------    --------");
	    mvprintw(y, x-24, "|  Quit  |  |  Resume  |  |  Top 10  |  |  Load  |");
	    mvprintw(y+1,x-24," --------    ----------    ----------    --------");

	    mvprintw(y+5, x-13, "SELECT");
	    mvprintw(y+6, x-17, " ____    ____"); 
	    mvprintw(y+7, x-17,  "||<-||  ||->||");
	    mvprintw(y+8, x-17, "||__||  ||__||");
 	    mvprintw(y+9,x-17, "|/__\\|  |/__\\|");
	    
	    mvprintw(y+5, x+6, "CONFIRM");
	    mvprintw(y+6, x+3, " ____________"); 
	    mvprintw(y+7, x+3, "||   ENTER  ||");
	    mvprintw(y+8, x+3, "||__________||");
 	    mvprintw(y+9, x+3, "|/__________\\|");

	    attron(COLOR_PAIR(BLUE));
	    switch (pause_menu_selection) {
		case 0:
		    mvprintw(y-1, x-24, " --------");
		    mvprintw(y, x-24, "|  Quit  |");
		    mvprintw(y+1, x-24, " --------");
		    break;
		case 1:
		    mvprintw(y-1, x-12, " ----------");
		    mvprintw(y, x-12, "|  Resume  |");
		    mvprintw(y+1, x-12, " ----------");
		    break;
		case 2:
		    mvprintw(y-1, x+2, " ----------");
		    mvprintw(y, x+2, "|  Top 10  |");
		    mvprintw(y+1, x+2, " ----------");
		    break;
		case 3:
		    mvprintw(y-1, x+16, " --------");
		    mvprintw(y, x+16, "|  Load  |");
		    mvprintw(y+1, x+16, " --------");
		    break;
	    }
	    attroff(COLOR_PAIR(BLUE));

	    ch = get_char();
	    switch (ch) {
		case 'p':
		case 'P':
		    state = ALIVE;
		    break;
		case LEFT:
		    if (pause_menu_selection > 0)
			pause_menu_selection -= 1;
		    break;
		case RIGHT:
		    if (pause_menu_selection < 3)
			pause_menu_selection += 1;
		    break;
		case '\n':
		    switch (pause_menu_selection) {
			case 0:
			    state = EXIT;
			    break;
			case 1:
			    state = ALIVE;
			    break;
			case 2:
			    state = TOP_10;
			    break;
			case 3:
			    free_snake(snake);
			    free_snake(enemy_snake);
			    free(window);
			    free_food(foods);
			    free_obstacles(obstacles);
			    state = LOAD;
			    break;
		    }
		    break;
	    }

	    break;

	case TOP_10:
	    clear(); 
	    x = window->upper_left_x + (window->width / 2);
	    y = window->upper_left_y + (window->height / 2);
	    draw_Gamewindow(window);

	    attron(COLOR_PAIR(BLUE));
	    mvprintw(y-12, x-14, " _____             __  _____ ");
	    mvprintw(y-11, x-14, "|_   _|           /  ||  _  |");
	    mvprintw(y-10, x-14, "  | | ___  _ __   `| || |/' |");
	    mvprintw(y-9,  x-14, "  | |/ _ \\| '_ \\   | ||  /| |");
	    mvprintw(y-8,  x-14, "  | | (_) | |_) | _| |\\ |_/ /");
	    mvprintw(y-7,  x-14, "  \\_/\\___/| .__/  \\___/\\___/ ");
	    mvprintw(y-6,  x-14, "          | |                ");
	    mvprintw(y-5,  x-14, "          |_|                ");
	    attroff(COLOR_PAIR(BLUE));

	    static int top_scores[10];
	    if (!checkedHighScores) {
		top_10(top_scores);
		checkedHighScores = true;
	    }
	    for (i = 0; i < 10; i++) {
		mvprintw((y-3)+i, x-2, "%d", top_scores[i]);
	    }

	    mvprintw(y+8, x-3, "RETURN");
	    mvprintw(y+9, x-7, " ____________"); 
	    mvprintw(y+10, x-7, "||   ENTER  ||");
	    mvprintw(y+11, x-7, "||__________||");
 	    mvprintw(y+12, x-7, "|/__________\\|");

	    ch = get_char();
	    if (ch == '\n') {
		state = PAUSED;
	    }
	    break;

        case INIT:
	    score = 0;
	    scoreThisLevel = 0;
	    checkedHighScores = false;
	    lives_remaining = 3;

	    // Setting height and width of the board
            x_offset = (x_max / 2) - (width / 2);
            y_offset = (y_max / 2) - (height / 2);
	    
	    // Init Game Window
	    window = init_GameWindow(x_offset, y_offset, width, height);
            draw_Gamewindow(window);

            // Init snake
            snake = init_snake(x_offset + (width / 2), y_offset + (height / 2) - 2);
	    enemy_snake = init_snake(x_offset + (width / 2), y_offset + (height / 2) + 2);
            
            // Init foods & obstacles
            int food_x, food_y, num_foods, num_obstacles, size;
            enum Type type;

	    // Change difficulty-specific settings
	    switch (difficulty) {
		case EASY:
		    num_foods = 5;
		    num_obstacles = 1;
		    break;
		case NORMAL:
		    num_foods = 10;
		    num_obstacles = 2;
		    timeret.tv_nsec /= 2;
		    break;
		case HARD:
		    num_foods = 20;
		    num_obstacles = 4;
		    timeret.tv_nsec /= 4;
		    break;
	    }

	    // Generate num_obstacles obstacles
	    size = (rand() % 4) + 1; // Randomly determine a size between 1 and 4.
	    generate_points(&food_x, &food_y, width - size, height - size, x_offset, y_offset); // we offset by size so bigger obstacles dont spawn outside the borders
            obstacles = create_obstacle(food_x, food_y, size);
            for(i = 1; i < num_obstacles; i++){
		size = (rand() % 4) + 1;
                generate_points(&food_x, &food_y, width - size, height - size, x_offset, y_offset);
                while (obstacle_exists(obstacles,food_x, food_y))
                    generate_points(&food_x, &food_y, width - size, height - size, x_offset, y_offset);
                new_obstacle = create_obstacle(food_x, food_y, size);
                add_new_obstacle(obstacles, new_obstacle);
            }

	    
            // Generate num_foods foods
            generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
            type = (rand() > RAND_MAX/2) ? Increase : Decrease; // Randomly deciding type of food
            foods = create_food(food_x, food_y, type);
            for(i = 1; i < num_foods; i++){
                generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
                while (food_exists(foods,food_x, food_y) || obstacle_exists(obstacles, food_x, food_y))
                    generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
                type = (rand() > RAND_MAX/2) ? Increase : Decrease;
                new_food = create_food(food_x, food_y, type);
                add_new_food(foods, new_food);
            }
	    snakeDir = NOCHAR;
            state = ALIVE;
	    extra_sleep_flag = display_lives(window, lives_remaining);
            break;

        case ALIVE:
	    // Get the end of the tail now BEFORE we move
	    tail = get_end(snake);
	    int tail_x = tail->x, tail_y = tail->y;
	    enemy_tail = get_end(enemy_snake);
	    int enemy_tail_x = enemy_tail->x, enemy_tail_y = enemy_tail->y;
	    bool freeall = false;
		
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

		// Resizing
		case '+':
		    if (boardIncreases < 3) {
			window = changeGameWindow(window->upper_left_x - 5, window->upper_left_y - 5, window->width + 10, window->height + 10, window);
			boardIncreases += 1;
		    }
		    break;
		case '-':
		    if (boardIncreases > -3) {
			window = changeGameWindow(window->upper_left_x + 5, window->upper_left_y + 5, window->width - 10, window->height - 10, window);
			boardIncreases -= 1;
		    }
		    break;

		// Pausing
		case 'p':
		case 'P':
		    checkedHighScores = false;
		    state = PAUSED;
		    break;
	
		// Loading 
		case 'l':
		case 'L':
			freeall = true;
			state = LOAD;
			break;


		// Saving
		case 'f':
		case 'F':;
			clear();
			// Create and open a new file "./saves/save_n.game", where n is the number of the save
			char fName[numSaves > 0 ? 19 + (int)(floor(log10(numSaves))) : 20];
			sprintf(fName, "./saves/save_%d.game", numSaves + 1);

			attron(COLOR_PAIR(GREEN));
			mvprintw(window->upper_left_y + (window->height / 2), window->upper_left_x + (window->width / 2) - 5, "SAVED AS #%d", numSaves + 1);
			attroff(COLOR_PAIR(GREEN));
			refresh();
			
			// Update the number of saves
			numSaves += 1;
			FILE *fPtr = fopen("./saves/num_saves.game", "wb");
			fwrite(&numSaves, sizeof(int), 1, fPtr);
			fclose(fPtr);
	
			// Write the save file
			fPtr = fopen(fName, "w");
			fprintf(fPtr, "score: %d\n", score);
			fprintf(fPtr, "lives_remaining: %d\n", lives_remaining);
			fprintf(fPtr, "scoreThisLevel: %d\n", scoreThisLevel);
			fprintf(fPtr, "boardIncreases: %d\n", boardIncreases);
			fprintf(fPtr, "timeret.tv_nsec: %Ld\n", timeret.tv_nsec);
			fprintf(fPtr, "snakeDir: %d\n", snakeDir);
			fprintf(fPtr, "enemySnakeDir: %d\n", enemySnakeDir);

			fprintf(fPtr, "snake: {");
			Snake *tempSnake = snake;
			while (tempSnake) {
				fprintf(fPtr, "x: %d, y: %d %c", tempSnake->x, tempSnake->y, tempSnake->next ? ';' : '}');
				tempSnake = tempSnake->next;
			}
			fprintf(fPtr, "\n");

			fprintf(fPtr, "enemy_snake: {");
			tempSnake = enemy_snake;
			while (tempSnake) {
				fprintf(fPtr, "x: %d, y: %d %c", tempSnake->x, tempSnake->y, tempSnake->next ? ';' : '}');
				tempSnake = tempSnake->next;
			}
			fprintf(fPtr, "\n");


			fprintf(fPtr, "foods: {");
			Food *tempFoods = foods;
			while (tempFoods) {
				fprintf(fPtr, "x: %d, y: %d, type: %c %c", tempFoods->x, tempFoods->y, tempFoods->type, tempFoods->next ? ';' : '}');
				tempFoods = tempFoods->next;
			}
			fprintf(fPtr, "\n");

			fprintf(fPtr, "obstacles: {");
			Obstacle *tempObst = obstacles;
			while (tempObst) {
				fprintf(fPtr, "x: %d, y: %d, size: %d %c", tempObst->x, tempObst->y, tempObst->size, tempObst->next ? ';' : '}');
				tempObst = tempObst->next;
			}
			fclose(fPtr);
			sleep(2);
			break;
	    }

	    // Move both snakes (if the player has entered a direction yet
	    if (snakeDir != NOCHAR) {
		snake = move_snake(snake, snakeDir);
	    }
	    // The AI snake will stay away from the edges, tending towards the center
	    if (snakeDir != NOCHAR) {
		int centerx = window->upper_left_x + (window->width / 2);
	    	int centery = window->upper_left_y + (window->height / 2);
		int maxDistanceFromCenterX = ((window->width / 2) - 1);
		int maxDistanceFromCenterY = ((window->height / 2) - 1);
		int distanceFromCenterX = abs(centerx - enemy_snake->x);
		int distanceFromCenterY = abs(centery - enemy_snake->y);

		int probability; // Calculate a number between 0 and 100 based on how far away from the center the snake is
		if (enemySnakeDir ==  UP || enemySnakeDir == DOWN) {
			probability = (int)(((float) distanceFromCenterY / (float) maxDistanceFromCenterY) * 100);
		}
		else {
			probability = (int)(((float) distanceFromCenterX / (float) maxDistanceFromCenterX) * 100);

		}
		// Check a random number against  the probability
		if ((rand() % 100) < probability) {
			if (enemySnakeDir == UP || enemySnakeDir == DOWN) {
				probability = 50 - (int)(((float)(centerx - enemy_snake->x) / (float)(maxDistanceFromCenterX)) * 50); 
				// Probability of making a left turn instead of right. Approaches 100 as snake gets closer to the right side
				enemySnakeDir = rand() % 100 < probability ? LEFT : RIGHT;
			}
			else {
				probability = 50 - (int)(((float)(centery - enemy_snake->y) / (float)(maxDistanceFromCenterY)) * 50); 
				enemySnakeDir = rand() % 100 < probability ? UP : DOWN;
			}
		}
		enemy_snake = move_snake(enemy_snake, enemySnakeDir);
	    }

	    // Check to see if the snake's head is now in a food
	    if (food_exists(foods, snake->x, snake->y)) {
		switch (remove_eaten_food(&foods, snake->x, snake->y)) {
		    case Increase:
			score += 20;
			scoreThisLevel += 20;
			if (scoreThisLevel >= 100) {
			    timeret.tv_nsec /= 1.5;
			    scoreThisLevel -= 100;
			}
			(get_end(snake))->next = create_tail(tail_x, tail_y); // Add another tail on the end
			break;
		    case Decrease:
			if (len(snake) > 1) {
			    score -= 10;
			    scoreThisLevel -= 10;
			    snake = remove_tail(snake);
			}
			else {
			    state = DEAD;
			}
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
	    // Check to see if the enemy snake's head is now in a food
	    if (food_exists(foods, enemy_snake->x, enemy_snake->y)) {
		switch (remove_eaten_food(&foods, enemy_snake->x, enemy_snake->y)) {
		    case Increase:
			if (len(enemy_snake) < window->width)
			    (get_end(enemy_snake))->next = create_tail(enemy_tail_x, enemy_tail_y); // Add another tail on the end
			break;
		    case Decrease:
			if (len(enemy_snake) > 1) {
			    enemy_snake = remove_tail(enemy_snake);
			}
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

		
	    // Check for collisions with walls
	    if (snake->x <= window->upper_left_x || snake->y <= window->upper_left_y || snake->x >= window->upper_left_x + window->width || snake->y >= window->upper_left_y + window->height) {
		checkedHighScores = false;
		state = DEAD;
	    }
	    // Check for collisions with obstacles
	    if (obstacle_exists(obstacles, snake->x, snake->y)) {
		checkedHighScores = false;
		state = DEAD;
	    }
	    // Check for collisions with the snake itself
	    if (len(snake) > 1 && eat_itself(snake)) {
		checkedHighScores = false;
		state = DEAD;
	    }
	    if (touching_snake(snake, enemy_snake)) {
		checkedHighScores = false;
		state = DEAD;
	    }

	    // Draw everything on the screen
            clear();
            mvprintw(20,20, "Key entered: %c", ch);
	    mvprintw(0, 0, "Score:  %d", score);
            draw_Gamewindow(window);
            draw_snake(snake, GREEN);
	    draw_snake(enemy_snake, MAGENTA);
	    draw_obstacles(obstacles);
            draw_food(foods);
	    if (freeall) {
		free_snake(snake);
		free_snake(enemy_snake);
		free(window);
		free_food(foods);
		free_obstacles(obstacles);
	    }
            break;

        case DEAD:
		clear();
		if (lives_remaining > 1) {
			lives_remaining -= 1;
			extra_sleep_flag = display_lives(window, lives_remaining);

			// Reset the snakes
			free_snake(snake);
			snake = init_snake(x_offset + (width / 2), y_offset + (height / 2));
			snakeDir = NOCHAR;
			enemySnakeDir = RIGHT;
			free_snake(enemy_snake);
			enemy_snake = init_snake(x_offset + (width / 2), y_offset + (height / 2) + 2);
			state = ALIVE;
		}
		else {
			attron(COLOR_PAIR(RED));
			mvprintw(0, 10, " _____                          _____");
			mvprintw(1, 10, "|  __ \\                        |  _  |");
			mvprintw(2, 10, "| |  \\/ __ _ _ __ ___   ___    | | | |");
			mvprintw(3, 10, "| | __ / _` | '_ ` _ \\ / _ \\   | | | \\ \\ / / _ \\ '__|");
			mvprintw(4, 10, "| |_\\ \\ (_| | | | | | |  __/   \\ \\_/ /\\ V /  __/ |");
			mvprintw(5, 10, " \\____/\\__,_|_| |_| |_|\\___|    \\___/  \\_/ \\___|_|");
			attroff(COLOR_PAIR(RED));

			mvprintw(7, 13,    "NEW GAME");
			mvprintw(8, 10, " ____________"); 
			mvprintw(9, 10, "||   ENTER  ||");
			mvprintw(10,10, "||__________||");
			mvprintw(11,10, "|/__________\\|");
			mvprintw(7, 55,  "QUIT");
			mvprintw(8, 54, " ____ ");
			mvprintw(9, 54, "||Q ||");
			mvprintw(10, 54,"||__||");
			mvprintw(11,54, "|/__\\|");

			mvprintw(15, 0, "Your Score:  %d", score);

			mvprintw(17, 0, "TOP 10");
			mvprintw(18, 0, "------");
			static int top10_scores[10];

			// We only want this to run once, not every loop.
			if (!checkedHighScores) {
				top_10(top10_scores);

				// Search through the array to see if this is a new high score.
				int newScoreIndex = -1;
				for (i = 0; i < 10; i++) {
					if (score > top10_scores[i]) {
						newScoreIndex = i;
						break;
					}
				}
				if (newScoreIndex != -1) { // A value that's not -1 signifies that we need to shift things around
					// Move the 9th element to place 10, 8th to place 9, etc, shifting down from newScoreIndex.
					for (i = 8; i >= newScoreIndex; i--) {
						top10_scores[i+1] = top10_scores[i];
					}
					top10_scores[newScoreIndex] = score; // Insert the score in the hole we just made

					// Update the file
					FILE *fPtr = fopen("./saves/save_best_10.game", "wb");
					fwrite(top10_scores, sizeof(int), 10, fPtr);
					fclose(fPtr);
				}
				checkedHighScores = true;
			}

			// Print out the top 10 scores
			for (i = 0; i < 10; i++) {
				mvprintw(19 + i, 0, "%d", top10_scores[i]);
			}

			ch = get_char();
			switch (ch) {
				case 'q':
				case 'Q':
					state = EXIT;
					break;
				case '\n':
					free_snake(snake);
					free_snake(enemy_snake);
					free(window);
					free_food(foods);
					free_obstacles(obstacles);
					state = DIFFICULTY_SELECT;
					break;
			}
		}
		break;
	case LOAD:
		nodelay(stdscr, FALSE);
		echo();
		clear();
		attron(COLOR_PAIR(BLUE));
		mvprintw(0, 0, " _                     _ ");
		mvprintw(1, 0, "| |                   | |");
		mvprintw(2, 0, "| |     ___   __ _  __| |");
		mvprintw(3, 0, "| |    / _ \\ / _` |/ _` |");
		mvprintw(4, 0, "| |___| (_) | (_| | (_| |");
		mvprintw(5, 0, "\\_____/\\___/ \\__,_|\\__,_|");
		attroff(COLOR_PAIR(BLUE));

		int save_num;
		mvprintw(8, 0, "File #: ");
		mvscanw(8, 8, "%d", &save_num);
		
		char fName2[save_num > 0 ? 19 + (int)(floor(log10(save_num))) : 20];
		sprintf(fName2, "./saves/save_%d.game", save_num);
		mvprintw(9, 0, "LOADING FROM '%s'", fName2);

		FILE *save = fopen(fName2, "r");
		if (save != NULL) {
			fscanf(save, "score: %d\n", &score);
			fscanf(save, "lives_remaining: %d\n", &lives_remaining);
			fscanf(save, "scoreThisLevel: %d\n", &scoreThisLevel);
			fscanf(save, "boardIncreases: %d\n", &boardIncreases);
			fscanf(save, "timeret.tv_nsec: %Ld\n", &(timeret.tv_nsec));
			fscanf(save, "snakeDir: %d\n", &snakeDir);
			fscanf(save, "enemySnakeDir: %d\n", &enemySnakeDir);

			int snakeX, snakeY;
			char endIndicator;
			fscanf(save, "snake: {x: %d, y: %d %c", &snakeX, &snakeY, &endIndicator);
			snake = create_tail(snakeX, snakeY);
			Snake *temp = snake;
			while (endIndicator == ';') {
				fscanf(save, "x: %d, y: %d %c", &snakeX, &snakeY, &endIndicator);
				temp->next = create_tail(snakeX, snakeY);
				temp = temp->next;
			}
			fscanf(save, "\n");

			fscanf(save, "enemy_snake: {x: %d, y: %d %c", &snakeX, &snakeY, &endIndicator);
			enemy_snake = create_tail(snakeX, snakeY);
			temp = enemy_snake;
			while (endIndicator == ';') {
				fscanf(save, "x: %d, y: %d %c", &snakeX, &snakeY, &endIndicator);
				temp->next = create_tail(snakeX, snakeY);
				temp = temp->next;
			}
			fscanf(save, "\n");

			char foodType;
			fscanf(save, "foods: {x: %d, y: %d, type: %c %c", &snakeX, &snakeY, &foodType, &endIndicator);
			foods = create_food(snakeX, snakeY, foodType == '+' || foodType == 'O' ? Increase : Decrease);
			Food *foodTemp = foods;
			while (endIndicator == ';') {
				fscanf(save, "x: %d, y: %d, type: %c %c", &snakeX, &snakeY, &foodType, &endIndicator);
				foodTemp->next = create_food(snakeX, snakeY, foodType == '+' || foodType == 'O' ? Increase : Decrease);
				foodTemp = foodTemp->next;
			}
			fscanf(save, "\n");
			
			int obstSize;
			fscanf(save, "obstacles: {x: %d, y: %d, size: %d %c", &snakeX, &snakeY, &obstSize, &endIndicator);
			obstacles = create_obstacle(snakeX, snakeY, obstSize);
			Obstacle *obstTemp = obstacles;
			while (endIndicator == ';') {
				fscanf(save, "x: %d, y: %d, size: %d %c", &snakeX, &snakeY, &obstSize, &endIndicator);
				obstTemp->next = create_obstacle(snakeX, snakeY, obstSize);
				obstTemp = obstTemp->next;
			}

			fclose(save);

			checkedHighScores = false;
			x_offset = (x_max / 2) - (width / 2);
			y_offset = (y_max / 2) - (height / 2);
			window = init_GameWindow(x_offset, y_offset, width, height);
			//window = changeGameWindow((window->upper_left_x - 5) * boardIncreases, (window->upper_left_y - 5) * boardIncreases, (window->width + 10) * boardIncreases, (window->height + 10) * boardIncreases, window);
			extra_sleep_flag = display_lives(window, lives_remaining);
			state = ALIVE; 
		}
		nodelay(stdscr, TRUE);
		noecho();
		break;
        }
        refresh();
        nanosleep(&timeret, NULL);
    }

    // Properly free all malloced resources before exiting
    if (snake)
	free_snake(snake);
    if (enemy_snake)
	free_snake(enemy_snake);
    if (window)
	free(window);
    if (foods)
	free_food(foods);
    if (obstacles)
	free_obstacles(obstacles);
    clear();
    endwin();
}

// Returns the number of saves that exist
// Useful if you want to know what to name the next save or if a save number is valid.
int num_saves() {
	int num_saves;
	FILE *fPtr = fopen("./saves/num_saves.game", "rb"); // Will point to ./saves/num_saves.game
	if (fPtr == NULL) {
		num_saves = 0;
		// Open the file in write mode and write 0 into it.
		fPtr = fopen("./saves/num_saves.game", "wb");
		fwrite(&num_saves, sizeof(int), 1, fPtr);
		fclose(fPtr);
	}
	else {
		fread(&num_saves, sizeof(int), 1, fPtr);
		fclose(fPtr);
	}
	return num_saves;
}

// Loads the top 10 scores into the array pointed to by top10Ptr
void top_10(int *top10Ptr) {
	FILE *fPtr = fopen("./saves/save_best_10.game", "rb");
	int top10[10] = { 0 };
	if (fPtr == NULL) { 
		fPtr = fopen("./saves/save_best_10.game", "wb");
		fwrite(top10, sizeof(int), 10, fPtr);
		fclose(fPtr);
	}
	else {
		fread(top10, sizeof(int), 10, fPtr);
		fclose(fPtr);
	}
	int i;
	for (i = 0; i < 10; i++) {
		top10Ptr[i] = top10[i];
	}
}

int display_lives(gamewindow_t *window, int lives_remaining) {
	clear();
	int x = window->upper_left_x + (window->width / 2);
	int y = window->upper_left_y + (window->height / 2);
	draw_Gamewindow(window);

	attron(COLOR_PAIR(RED));
	int i;
	for (i = 0; i < lives_remaining; i++) {
		mvprintw(y-2, (x-20) + (15 * i), ",d88b.d88b,");
		mvprintw(y-1, (x-20) + (15 * i), "88888888888");
		mvprintw(y ,  (x-20) + (15 * i), "`Y8888888Y'");
		mvprintw(y+1, (x-20) + (15 * i), "  `Y888Y'");
		mvprintw(y+2, (x-20) + (15 * i), "    `Y'");
	}
	attroff(COLOR_PAIR(RED));
	return 2;
}
