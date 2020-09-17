/* A program that makes a simple airline booking system.   *
 * Solves FA20 CSCI-C 291 HW2 Part 2                       *
 * Author: Zachary E Graber (zegraber@iu.edu)              *
 * Date: 9/16/2020 					   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define ROWS 15
#define SEATS_PER_ROW 7

// This will be used to keep track of malloced pointers for freeing
char* namePointers[ROWS * SEATS_PER_ROW] = { NULL };
int numNamePointers = 0;

struct Seat {
	int seatNo;
	char letter;
	char* name;
};

struct Pair {
	int i;
	int j;
};

int numEmptySeatsBetweenRows(int seatMatrix[ROWS][SEATS_PER_ROW], int r1, int r2);
int numFirstClassSeats(int seatMatrix[ROWS][SEATS_PER_ROW]);
int numBusinessClassSeats(int seatMatrix[ROWS][SEATS_PER_ROW]);
int numEconomyClassSeats(int seatMatrix[ROWS][SEATS_PER_ROW]);
int numPriorityEconomySeats(int seatMatrix[ROWS][SEATS_PER_ROW]);
bool isWindowSeat(struct Seat seat);
bool isAisleSeat(struct Seat seat);
struct Seat convertMatrixEntryToSeat(int seatMatrix[ROWS][SEATS_PER_ROW], int i, int j, char passengerName[]);
struct Pair getEmptySeatBetweenRows(int seatMatrix[ROWS][SEATS_PER_ROW], int r1, int r2);
char* mallocName(char passengerName[]);
void freeMallocedNames(void);

int main(void) {

	srand(time(NULL)); // Seed the random number generator
	char mmInput;
	printf("Type 'F' for First Class\n");
	printf("Type 'B' for Business Class\n");
	printf("Type anything else for Economy\n");
	scanf(" %c", &mmInput);
	
	int seatMatrix[ROWS][SEATS_PER_ROW] = { {0} };

	struct Pair initAssnmt;
	switch(mmInput) {
		case 'F':
			//TODO
			break;

		case 'B':
			//TODO
			break;

		// Default case represents any other input
		default:
			//TODO
			break;
	}
	struct Pair testSeat = getEmptySeatBetweenRows(seatMatrix, 0, 2);
	printf("Random Seat between row 0 and 2: (%d, %d)", testSeat.i, testSeat.j);

	freeMallocedNames();
	return(0);
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

bool isWindowSeat(struct Seat seat) {
	bool result = false;
	if (seat.letter == 'A' || seat.letter == 'I') {
		result = true;
	}
	return result;	
}

bool isAisleSeat(struct Seat seat) {
	bool result = true;
	if (isWindowSeat(seat) || seat.letter == 'E') {
		result = false;
	}
	return result;
}

/*
 *  Returns a Seat struct containing the information specified.
 *  This function is useful for getting the name of a seat based on its matrix indices (i and j). 
 */
struct Seat convertMatrixEntryToSeat(int seatMatrix[ROWS][SEATS_PER_ROW], int i, int j, char passengerName[]) {
	struct Seat s;
	s.seatNo = (SEATS_PER_ROW * i) + j + 1;
	switch (j) {
		case 0:
		s.letter = 'A';
		break;

		case 1:
		s.letter = 'B';
		break;
		
		case 2:
		s.letter = 'D';
		break;

		case 3:
		s.letter = 'E';
		break;

		case 4:
		s.letter = 'F';
		break;

		case 5:
		s.letter = 'H';
		break;

		case 6:
		s.letter = 'I';
		break;
	}
	s.name = mallocName(passengerName);
	return s;	
}

/*
 *  Gets the matrix coordinates (indices) of a random empty seat between rows r1 and r2 (inclusive) as a Pair struct
 */
struct Pair getEmptySeatBetweenRows(int seatMatrix[ROWS][SEATS_PER_ROW], int r1, int r2) {
	struct Pair p;
	int numEmptySeats = numEmptySeatsBetweenRows(seatMatrix, r1, r2);
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
	return emptySeats[rand() % numEmptySeats];
}

/*
 *  malloc()'s a pointer for a name and registers it to be freed when the program exits.
 */
char* mallocName(char passengerName[]) {
	char* ptr = (char*) malloc(sizeof(passengerName));
	namePointers[numNamePointers] = ptr;
	numNamePointers += 1;
	return ptr;	
}

/*
 *  Frees pointers previously allocated for seat's names.
 */
void freeMallocedNames(void) {
	int i;
	for (i = 0; i < numNamePointers; i++) {
		free(namePointers[i]);
	}
}
