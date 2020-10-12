/*
 * 
 *   Name: game.c
 *   Contents: Contains all the main logic and main loop for the game
 *   Original Author Unknown.
 *   Most recent edit: 
 *   	Date: 10/11/2020
 *	Editor: Zachary E Graber (zegraber@iu.edu)
 *
 */

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "snake.h"
#include "food.h"
#include "game_window.h"
#include "key.h"
#include "game.h"
#include "obstacle.h"

void generate_points(int *food_x, int *food_y, int width, int height, int x_offset, int y_offset){
    *food_x = rand() % width + x_offset;
    *food_y = rand() % height + y_offset;
}

void game(){
    enum State state = MENU; // Set the initial state
    enum Difficulty difficulty = NORMAL; // Difficulty of the game.
    static int x_max, y_max; //Max screen size variables
    static int x_offset, y_offset; // distance between the top left corner of your screen and the start of the board
    gamewindow_t *window; // Name of the board
    Snake *snake; // The snake
    Food *foods,*new_food; // List of foods (Not an array)
    Obstacle *obstacles, *new_obstacle; // List of obstacles.
    int snakeDir; // The current direction the snake is moving.
    Snake *endOfTail; // The end of the snake's tail
    int endX; // X location of end of tail
    int endY; // Y location of end of tail
    int score;
    int scoreThisLevel;
    int boardIncreases = 0;
    int numSaves = num_saves();
    bool checkedHighScores;

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
	    mvprintw(0, 20, "______ _  __  __ _            _ _");
	    mvprintw(1, 20, "|  _  (_)/ _|/ _(_)          | | |");
	    mvprintw(2, 20, "| | | |_| |_| |_ _  ___ _   _| | |_ _   _ ");
	    mvprintw(3, 20, "| | | | |  _|  _| |/ __| | | | | __| | | |");
	    mvprintw(4, 20, "| |/ /| | | | | | | (__| |_| | | |_| |_| |");
	    mvprintw(5, 20, "|___/ |_|_| |_| |_|\\___|\\__,_|_|\\__|\\__, |");
	    mvprintw(6, 20, "                                     __/ |");
	    mvprintw(7, 20, "                                    |___/ ");

	    mvprintw(12, 38, "NORMAL");
	    mvprintw(12, 20, "EASY");
	    mvprintw(12, 57, "HARD");
	    
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
	    mvprintw(10, selectionBoxX, " --------");
	    int i;
	    for (i = 11; i < 14; i++) {
		mvprintw(i, selectionBoxX, "|");
		mvprintw(i, selectionBoxX + 9, "|");
	    }
	    mvprintw(14, selectionBoxX, " --------");
	
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

	    mvprintw(y-6, x-15, "______                        _ ");
	    mvprintw(y-5, x-15, "| ___ \\                      | |");
	    mvprintw(y-4, x-15, "| |_/ /_ _ _   _ ___  ___  __| |");
	    mvprintw(y-3, x-15, "|  __/ _` | | | / __|/ _ \\/ _` |");
	    mvprintw(y-2, x-15, "| | | (_| | |_| \\__ \\  __/ (_| |");
	    mvprintw(y-1, x-15, "\\_|  \\__,_|\\__,_|___/\\___|\\__,_|");

	    mvprintw(y+2, x-3, " ____ "); 
	    mvprintw(y+3, x-3, "||P ||");
	    mvprintw(y+4, x-3, "||__||");
 	    mvprintw(y+5, x-3, "|/__\\|");
	    mvprintw(y+7, x-3, "UNPAUSE");

	    ch = get_char();
	    if (ch == 'p' || ch == 'P') {
		state = ALIVE;
	    }

	    break;

        case INIT:
	    score = 0;
	    scoreThisLevel = 0;
	    checkedHighScores = false;

	    // Setting height and width of the board
            x_offset = (x_max / 2) - (width / 2);
            y_offset = (y_max / 2) - (height / 2);
	    
	    // Init Game Window
	    window = init_GameWindow(x_offset, y_offset, width, height);
            draw_Gamewindow(window);

            // Init snake
            snake = init_snake(x_offset + (width / 2), y_offset + (height / 2));
            
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
            break;

        case ALIVE:
	    endOfTail = get_end(snake);
	    endX = endOfTail->x;
	    endY = endOfTail->y;

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
		    state = PAUSED;
		    break;

		// Saving
		case 'f':
		case 'F':;
			// Create and open a new file "./saves/save_n.game", where n is the number of the save
			char fName[numSaves > 0 ? 19 + (int)(floor(log10(numSaves))) : 20];
			sprintf(fName, "./saves/save_%d.game", numSaves + 1);
			
			// Update the number of saves
			numSaves += 1;
			FILE *fPtr = fopen("./saves/num_saves.game", "wb");
			fwrite(&numSaves, sizeof(int), 1, fPtr);
			fclose(fPtr);

			// Write the save file
			fPtr = fopen(fName, "w");
			fprintf(fPtr, "score: %d\n", score);
			fprintf(fPtr, "scoreThisLevel: %d\n", scoreThisLevel);
			fprintf(fPtr, "boardIncreases: %d\n", boardIncreases);
			fprintf(fPtr, "timeret.tv_nsec: %Ld\n", timeret.tv_nsec);
			fprintf(fPtr, "snakeDir: %d\n", snakeDir);

			fprintf(fPtr, "snake: {");
			Snake *tempSnake = snake;
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
			break;
	    }

	    // Move the snake (if the player has entered a direction yet
	    if (snakeDir != NOCHAR) {
		snake = move_snake(snake, snakeDir);
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
			(get_end(snake))->next = create_tail(endX, endY); // Add another tail on the end
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
		
	    // Check for collisions with walls
	    if (snake->x <= window->upper_left_x || snake->y <= window->upper_left_y || snake->x >= window->upper_left_x + window->width || snake->y >= window->upper_left_y + window->height) {
		state = DEAD;
	    }
	    // Check for collisions with obstacles
	    if (obstacle_exists(obstacles, snake->x, snake->y)) {
		state = DEAD;
	    }
	    // Check for collisions with the snake itself
	    if (len(snake) > 1 && eat_itself(snake)) {
		state = DEAD;
	    }

	    // Draw everything on the screen
            clear();
            mvprintw(20,20, "Key entered: %c", ch);
	    mvprintw(0, 0, "Score:  %d", score);
            draw_Gamewindow(window);
            draw_snake(snake);
	    draw_obstacles(obstacles);
            draw_food(foods);
            break;

        case DEAD:
		clear();
		mvprintw(0, 10, " _____                          _____");
		mvprintw(1, 10, "|  __ \\                        |  _  |");
		mvprintw(2, 10, "| |  \\/ __ _ _ __ ___   ___    | | | |");
		mvprintw(3, 10, "| | __ / _` | '_ ` _ \\ / _ \\   | | | \\ \\ / / _ \\ '__|");
		mvprintw(4, 10, "| |_\\ \\ (_| | | | | | |  __/   \\ \\_/ /\\ V /  __/ |");
		mvprintw(5, 10, " \\____/\\__,_|_| |_| |_|\\___|    \\___/  \\_/ \\___|_|");

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
				FILE *fPtr = fopen("./saves/saves_best_10.game", "wb");
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
				state = INIT;
				break;
		}
		break;
        }
        refresh();
        nanosleep(&timeret, NULL);
    }
    clear();
    endwin();
}

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

void top_10(int *top10Ptr) {
	FILE *fPtr = fopen("./saves/saves_best_10.game", "rb");
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
