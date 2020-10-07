/* A program that makes a simple airline booking system.   *
 * Solves FA20 CSCI-C 291 HW2 Part 2                       *
 * Author: Zachary E Graber (zegraber@iu.edu)              *
 * Date: 9/16/2020 					   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define ROWS 15
#define SEATS_PER_ROW 7

struct Seat {
	int seatNo;
	char letter;
	char* name;
};

struct Pair {
	int i;
	int j;
};

typedef struct Seat Seat;
typedef struct Pair Pair;

// Initialize an array to hold a pointer to every Seat struct for use later.
// This is a flattened version of the 2d int array, meaning each [i][j] index corresponds to some [i * ROWS + j] here
Seat *seatPointersArr[ROWS * SEATS_PER_ROW] = { NULL };

// Function prototypes
bool confirmInitSelection(Seat *assignedSeat);
int numEmptySeatsBetweenRows(int seatMatrix[ROWS][SEATS_PER_ROW], int r1, int r2);
int numFirstClassSeats(int seatMatrix[ROWS][SEATS_PER_ROW]);
int numBusinessClassSeats(int seatMatrix[ROWS][SEATS_PER_ROW]);
int numEconomyClassSeats(int seatMatrix[ROWS][SEATS_PER_ROW]);
int numPriorityEconomySeats(int seatMatrix[ROWS][SEATS_PER_ROW]);
bool isWindowSeat(struct Seat *seat);
bool isAisleSeat(struct Seat *seat);
struct Seat* convertMatrixEntryToSeat(int seatMatrix[ROWS][SEATS_PER_ROW], int i, int j);
struct Pair* getEmptySeatBetweenRows(int seatMatrix[ROWS][SEATS_PER_ROW], int r1, int r2);
char* getNameInput();
void registerSeatInGlobalArray(Seat* seat, Pair* pos);
void freeMallocedSeats(void);


int main(void) {
	srand(time(NULL)); // Seed the random number generator
	char mmInput;
	int seatMatrix[ROWS][SEATS_PER_ROW] = { {0} };

	bool sentinel = true;
	while (sentinel) {
		printf("Type 'F' for First Class\n");
		printf("Type 'B' for Business Class\n");
		printf("Type 'Q' to quit\n");
		printf("Type anything else for Economy\n");
		scanf(" %c", &mmInput);

		bool userSatisfied = false;
		struct Pair *assnmt;
	
		// If every seat is full already
		if (numEmptySeatsBetweenRows(seatMatrix, 0, ROWS - 1) == 0) {
			printf("Sorry, but all seats are full. Next flight leaves in 3 hours.\n");
		}

		// Keep going until the user is satisfied.
		while (!userSatisfied) {
			switch(mmInput) {
				case 'F':
					if (numFirstClassSeats(seatMatrix) > 0) {
						assnmt = getEmptySeatBetweenRows(seatMatrix, 1, 3);
						Seat *assignedSeat = convertMatrixEntryToSeat(seatMatrix, assnmt->i, assnmt->j);
						if (confirmInitSelection(assignedSeat)) {
							char usrInput;
							printf("Would you like to downgrade to Business class? (y / n)\n");
							scanf(" %c", &usrInput);
							if (usrInput == 'Y' || usrInput == 'y') {
								// TODO: Get a downgraded seat (if possible) and choose between them !!!FREE THE ONE NOT USED!!!
							}
							else {
								assignedSeat->name = getNameInput();
								registerSeatInGlobalArray(assignedSeat, assnmt); // Puts the heap-stored ptr for the seat in seatPointersArr
								userSatisfied = true;
							}
						}
						else {
							// If they don't like their initial selection, free the seat then do it again.
							free(assignedSeat);
						}
						free(assnmt); // The Pair assnmt is no longer needed so it is freed.
					}
					// If first class is full
					else {
						char usrInput;
						printf("Sorry, but First Class is full. Would you like to downgrade?\n\t> Business class (B)\n\t> Economy (E)\n\t> Cancel (C)\n");
						scanf(" %c", &usrInput);
						switch (usrInput) {
							case 'B':
								mmInput = 'B';
								break;
							case 'E':
								mmInput = 'E';
								break;
							default:
								mmInput = 'Q';
								break;
						}
					}
					break;

				case 'B':
					//TODO
					break;
				case 'q':
				case 'Q':
					userSatisfied = true;
					sentinel = false;
					break;
				// Default case represents any other input (economy)
				default:
					//TODO
					break;
			}
		}
	}

	freeMallocedSeats();
	return(0);
}

bool confirmInitSelection(Seat *assignedSeat) {
	printf("Is Seat %c%d okay? (y / n)\n", assignedSeat->letter, assignedSeat->seatNo);
	if (isWindowSeat(assignedSeat)) { printf("This seat is a window seat.\n"); }
	else if (isAisleSeat(assignedSeat)) { printf("This seat is an aisle seat.\n"); }
	char usrInput;
	while (1) {
		scanf(" %c", &usrInput);
		switch (usrInput) {
			case 'Y':
			case 'y':
				return true;
				break;
			case 'N':
			case 'n':
				return false;
				break;
		}
	}
}

/*
 *  Gives the number of empty seats from row r1 (inclusive) to row r2 (inclusive)
 */
int numEmptySeatsBetweenRows(int seatMatrix[ROWS][SEATS_PER_ROW], int r1, int r2) {
	int numSeats = 0;
	int i;
	int j;
	for (i = r1; i <= r2; i++) {
		for (j = 0; j < SEATS_PER_ROW; j++) {
			if (seatMatrix[i][j] == 0) {
				numSeats += 1;
			}
		}
	}
	return numSeats;

}

int numFirstClassSeats(int seatMatrix[ROWS][SEATS_PER_ROW]) { return numEmptySeatsBetweenRows(seatMatrix, 0, 2); }

int numBusinessClassSeats(int seatMatrix[ROWS][SEATS_PER_ROW]) { return numEmptySeatsBetweenRows(seatMatrix, 3, 6); }

int numEconomyClassSeats(int seatMatrix[ROWS][SEATS_PER_ROW]) { return numEmptySeatsBetweenRows(seatMatrix, 7, 14); }

int numPriorityEconomySeats(int seatMatrix[ROWS][SEATS_PER_ROW]) { return numEmptySeatsBetweenRows(seatMatrix, 7, 7); }

bool isWindowSeat(struct Seat *seat) {
	bool result = false;
	if (seat->letter == 'A' || seat->letter == 'I') {
		result = true;
	}
	return result;	
}

bool isAisleSeat(struct Seat *seat) {
	bool result = true;
	if (isWindowSeat(seat) || seat->letter == 'E') {
		result = false;
	}
	return result;
}

/*
 *  Returns a pointer to a Seat struct on the heap, containing the information specified.
 *  Keep in mind that the pointer returned at its name member are malloced and must be freed!
 *  This function is especially useful for getting the name of a seat based on its matrix indices (i and j). 
 */
struct Seat* convertMatrixEntryToSeat(int seatMatrix[ROWS][SEATS_PER_ROW], int i, int j) {
	Seat *s = (Seat*) malloc(sizeof(s));
	s->seatNo = (SEATS_PER_ROW * i) + j + 1;
	switch (j) {
		case 0:
		s->letter = 'A';
		break;

		case 1:
		s->letter = 'B';
		break;
		
		case 2:
		s->letter = 'D';
		break;

		case 3:
		s->letter = 'E';
		break;

		case 4:
		s->letter = 'F';
		break;

		case 5:
		s->letter = 'H';
		break;

		case 6:
		s->letter = 'I';
		break;
	}
	s->name = NULL;
	return s;	
}

/*
 *  Gets the matrix coordinates (indices) of a random empty seat between rows r1 and r2 (inclusive) as a pointer to a Pair struct on the heap.
 *  Keep in mind that these must be freed when you're done with them!
 */
struct Pair* getEmptySeatBetweenRows(int seatMatrix[ROWS][SEATS_PER_ROW], int r1, int r2) {
	int numEmptySeats = numEmptySeatsBetweenRows(seatMatrix, r1, r2);
	
	// Build an array of all possible seats
	struct Pair emptySeats[numEmptySeats];
	int i;
	int j;
	int k = 0;
	for (i = r1; i <= r2; i++) {
		for (j = 0; j < SEATS_PER_ROW; j++) {
			if (seatMatrix[i][j] == 0) {
				emptySeats[k].i = i;
				emptySeats[k].j = j;
				k += 1;
			}
		}
	}
	int randIndex = (rand() % numEmptySeats); // Randomly choose one of the available seats

	// Build the returned Pair structure
	Pair *p = (Pair*) malloc(sizeof(p));
	p->i = emptySeats[randIndex].i;
	p->j = emptySeats[randIndex].j;
	return p;
}

/*
 *  malloc()'s a pointer for a name.
 */
char* getNameInput() {
	// scanf user input into a 255 character-max C string as a temp, then malloc the appropriate size and copy
	char temp[256];
	printf("Please enter your name:  ");
	scanf(" %255s", temp);

	char* ptr = (char*) malloc(strlen(temp) + 1); // add 1 for '\0' character
	strcpy(ptr, temp);
	return ptr;	
}

void registerSeatInGlobalArray(Seat* seat, Pair* pos) {
	seatPointersArr[(pos->i * ROWS) + pos->j] = seat;
}

/*
 *  Frees pointers previously allocated for seat's names.
 */
void freeMallocedSeats(void) {
	// Free all the names stored in the Seat structs first, then the Seats themselves
	int i;
	for (i = 0; i < (ROWS * SEATS_PER_ROW); i++) {
		if (seatPointersArr[i] != NULL) {
			free(seatPointersArr[i]->name);
			free(seatPointersArr[i]);
		}
	}
}
