/* A program that makes an airline booking system.         *
 * Solves FA20 CSCI-C 291 HW2 Part 2                       *
 * Author: Zachary E Graber (zegraber@iu.edu)              *
 * Date: 10/7/2020 					   *
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
void printConfirmation(Seat* seat);
void printSeatingChart(int seatMatrix[ROWS][SEATS_PER_ROW]);
void printManifest(void);
void printBoardingPass(char letter, int seatNo);
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
		Seat *assignedSeat;
	
		// If every seat is full already
		if (numEmptySeatsBetweenRows(seatMatrix, 0, ROWS - 1) == 0) {
			printf("Sorry, but all seats are full. Next flight leaves in 3 hours.\n");
		}

		// Keep going until the user is satisfied.
		while (!userSatisfied) {
			switch(mmInput) {
				// First Class Booking
				case 'F':
					if (numFirstClassSeats(seatMatrix) > 0) {
						assnmt = getEmptySeatBetweenRows(seatMatrix, 0, 2);
						assignedSeat = convertMatrixEntryToSeat(seatMatrix, assnmt->i, assnmt->j);
						if (confirmInitSelection(assignedSeat)) {
							char usrInput;
							printf("Would you like to downgrade to Business class? (y / n)\n");
							scanf(" %c", &usrInput);
							if (usrInput == 'Y' || usrInput == 'y') {
								// Get a downgraded seat (if possible) and choose between them !!!FREE THE ONE NOT USED!!!
								if (numBusinessClassSeats(seatMatrix) > 0) {
									do {
										free(assignedSeat);
										free(assnmt);
										assnmt = getEmptySeatBetweenRows(seatMatrix, 3, 6);
										assignedSeat = convertMatrixEntryToSeat(seatMatrix, assnmt->i, assnmt->j);
										printConfirmation(assignedSeat);
									} while (!confirmInitSelection(assignedSeat));
									assignedSeat->name = getNameInput();
									registerSeatInGlobalArray(assignedSeat, assnmt);
									seatMatrix[assnmt->i][assnmt->j] = 1;
									userSatisfied = true;
									printConfirmation(assignedSeat);
								}
								// Business class is full
								else {
									printf("Sorry, but Business class is full. Would you like to...\n\t> Book seat %c%d in First class ('Y')\n\t> Cancel (Anything else)\n", assignedSeat->letter, assignedSeat->seatNo);
									scanf(" %c", &usrInput);
									if (usrInput == 'Y' || usrInput == 'y') {
										assignedSeat->name = getNameInput();
										registerSeatInGlobalArray(assignedSeat, assnmt);
										seatMatrix[assnmt->i][assnmt->j] = 1;
										userSatisfied = true;
										printConfirmation(assignedSeat);
									}
									else {
										free(assignedSeat);
										userSatisfied = true;
									}
								}
							}
							else {
								assignedSeat->name = getNameInput();
								registerSeatInGlobalArray(assignedSeat, assnmt); // Puts the heap ptr for the seat in seatPointersArr
								seatMatrix[assnmt->i][assnmt->j] = 1; // Mark the seat as occupied
								printConfirmation(assignedSeat);
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

				// Business class booking
				case 'B':
					if (numBusinessClassSeats(seatMatrix) > 0) {
						assnmt = getEmptySeatBetweenRows(seatMatrix, 3, 6);
						assignedSeat = convertMatrixEntryToSeat(seatMatrix, assnmt->i, assnmt->j);
						if (confirmInitSelection(assignedSeat)) {
							char usrInput;
							printf("Would you like to...\n\t> Upgrade to First class (F)\n\t> Downgrade to Economy class (E)\n\t> No thanks (N)\n");
							scanf(" %c", &usrInput);
							switch (usrInput) {
								case 'f':
								case 'F':
									if (numFirstClassSeats(seatMatrix) > 0) {
										free(assignedSeat);
										mmInput = 'F';
									}
									else {
										printf("Sorry, but First class is full.\nWould you still like to book seat %c%d (y / n)?\n", assignedSeat->letter, assignedSeat->seatNo);
										scanf(" %c", &usrInput);
										if (usrInput == 'Y' || usrInput == 'y') {
											assignedSeat->name = getNameInput();
											registerSeatInGlobalArray(assignedSeat, assnmt);
											seatMatrix[assnmt->i][assnmt->j] = 1;
											userSatisfied = true;
											printConfirmation(assignedSeat);
										}
										else {
											free(assignedSeat);
											userSatisfied = true;
										}
									}
									break;
								case 'e':
								case 'E':
									if (numEconomyClassSeats(seatMatrix) > 0) {
										free(assignedSeat);
										mmInput = 'E';
									}
									else {
										printf("Sorry, but Economy class is full.\nWould you still like to book seat %c%d (y / n)?\n", assignedSeat->letter, assignedSeat->seatNo);
										scanf(" %c", &usrInput);
										if (usrInput == 'Y' || usrInput == 'y') {
											assignedSeat->name = getNameInput();
											registerSeatInGlobalArray(assignedSeat, assnmt);
											seatMatrix[assnmt->i][assnmt->j] = 1;
											userSatisfied = true;
											printConfirmation(assignedSeat);
										}
										else {
											free(assignedSeat);
											userSatisfied = true;
										}
									}
									break;
								default:
									assignedSeat->name = getNameInput();
									registerSeatInGlobalArray(assignedSeat, assnmt);
									seatMatrix[assnmt->i][assnmt->j] = 1;
									userSatisfied = true;
									printConfirmation(assignedSeat);
									break;
							}
						}
						else {
							free(assignedSeat); // Free this assignment and let them pick a new one
						}
						free(assnmt);
					}
					// If Business class is full, offer a downgrade to economy
					else {
						printf("Sorry, but Business class is full. Would you like to downgrade?\n\t> Economy (Y)\n\t> Cancel (N)\n");
						char usrInput;
						scanf(" %s", &usrInput);
						if (usrInput == 'Y' || usrInput == 'y') {
							mmInput = 'E';
						}
						else {
							mmInput = 'Q';
						}
					}
					break;
				case 'q':
				case 'Q':
					userSatisfied = true;
					//sentinel = false;
					freeMallocedSeats();
					return(0);
					break;

				// Default case represents any other input (economy)
				// Economy class booking
				default: ;
					char usrInput = 'N';
					if (numEconomyClassSeats(seatMatrix) > 0) {
						if (numPriorityEconomySeats(seatMatrix) > 0) {
							printf("Would you like Priority Economy seating? (y / n)\n");
							scanf(" %c", &usrInput);
						}

						if (usrInput == 'Y' || usrInput == 'y') {
							assnmt = getEmptySeatBetweenRows(seatMatrix, 7, 7);
							assignedSeat = convertMatrixEntryToSeat(seatMatrix, assnmt->i, assnmt->j);
							while (!confirmInitSelection(assignedSeat)) {
								free(assnmt);
								free(assignedSeat);
								assnmt = getEmptySeatBetweenRows(seatMatrix, 7, 7);
								assignedSeat = convertMatrixEntryToSeat(seatMatrix, assnmt->i, assnmt->j);
							}
						}
						else {
							assnmt = getEmptySeatBetweenRows(seatMatrix, 7, 14);
							assignedSeat = convertMatrixEntryToSeat(seatMatrix, assnmt->i, assnmt->j);
							while (!confirmInitSelection(assignedSeat)) {
								free(assnmt);
								free(assignedSeat);
								assnmt = getEmptySeatBetweenRows(seatMatrix, 7, 14);
								assignedSeat = convertMatrixEntryToSeat(seatMatrix, assnmt->i, assnmt->j);
							}
						}
						
						// Now that we have a seat, we ask about upgrades.
						printf("Would you like to upgrade to...\n\t> First Class (F)\n\t> Business Class (B)\n\t> No thanks (N)\n");
						scanf(" %c", &usrInput);
						switch (usrInput) {
							case 'f':
							case 'F':
								free(assignedSeat);
								mmInput = 'F';
								break;
							case 'b':
							case 'B':
								free(assignedSeat);
								mmInput = 'B';
								break;
							default:
									assignedSeat->name = getNameInput();
									registerSeatInGlobalArray(assignedSeat, assnmt);
									seatMatrix[assnmt->i][assnmt->j] = 1;
									userSatisfied = true;
									printConfirmation(assignedSeat);
								break;
						}
						free (assnmt);
					}
					else {
						printf("Sorry, but Economy class is full. Would you like to upgrade?\n\t> Business (B)\n\t> First (F)\n\t> Cancel (C)\n");
						scanf(" %c", &usrInput);
						switch (usrInput) {
							case 'b':
							case 'B':
								mmInput = 'B';
								break;
							case 'f':
							case 'F':
								mmInput = 'F';
								break;
							default:
								mmInput = 'Q';
								break;
						}
					}
					break;
			}
		}
		
		// Post-Booking options
		userSatisfied = false;
		while (!userSatisfied) {
			char usrInput;
			printf("Options\n-------\n\t(1) Show seating chart\n\t(2) Print manifest\n\t(3) Print boarding pass\n\t(other) Book another ticket\n");
			scanf(" %c", &usrInput);
			switch (usrInput) {
				case '1':
					printSeatingChart(seatMatrix);
					break;
				case '2':
					printManifest();
					break;
				case '3': ;
					char letter;
					int seatNo;
					printf("Enter the seat number (e.g. A1):  ");
					scanf(" %c%d", &letter, &seatNo);
					printBoardingPass(letter, seatNo);
					break;
				default:
					userSatisfied = true;
					break;
			}
		}
	}
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
	scanf(" %255[^\n]s", temp);

	char* ptr = (char*) malloc(strlen(temp) + 1); // add 1 for '\0' character
	strcpy(ptr, temp);
	return ptr;	
}

void registerSeatInGlobalArray(Seat* seat, Pair* pos) {
	seatPointersArr[(pos->i * ROWS) + pos->j] = seat;
}

void printConfirmation(Seat *seat) {
	printf("\nSuccesfully booked seat %c%d for %s!\n\n", seat->letter, seat->seatNo, seat->name);
}

void printSeatingChart(int seatMatrix[ROWS][SEATS_PER_ROW]) {
	printf("O = empty, X = occupied\n");
	printf("A\t");
	int i;
	for (i = 0; i < ROWS; i++) {
		printf("%c ", seatMatrix[i][0] == 1 ? 'X' : 'O');
	}
	printf("\n");
	printf("B\t");
	for (i = 0; i < ROWS; i++) {
		printf("%c ", seatMatrix[i][1] == 1 ? 'X' : 'O');
	}
	printf("\n\n");
	printf("D\t");
	for (i = 0; i < ROWS; i++) {
		printf("%c ", seatMatrix[i][2] == 1 ? 'X' : 'O');
	}
	printf("\n");
	printf("E\t");
	for (i = 0; i < ROWS; i++) {
		printf("%c ", seatMatrix[i][3] == 1 ? 'X' : 'O');
	}
	printf("\n");
	printf("F\t");
	for (i = 0; i < ROWS; i++) {
		printf("%c ", seatMatrix[i][4] == 1 ? 'X' : 'O');
	}
	printf("\n\n");
	printf("H\t");
	for (i = 0; i < ROWS; i++) {
		printf("%c ", seatMatrix[i][5] == 1 ? 'X' : 'O');
	}
	printf("\n");
	printf("I\t");
	for (i = 0; i < ROWS; i++) {
		printf("%c ", seatMatrix[i][6] == 1 ? 'X' : 'O');
	}
	printf("\n");
}

void printManifest(void) {
	printf("\nMANIFEST\n\n%-32s %4s\n-------------------------------------\n", "Name", "Seat");
	int i;
	for (i = 0; i < (ROWS * SEATS_PER_ROW); i++) {
		if (seatPointersArr[i] != NULL) {
			printf("%-32s %c%d\n", seatPointersArr[i]->name, seatPointersArr[i]->letter, seatPointersArr[i]->seatNo);
		}
	}
	printf("\n");
}

void printBoardingPass(char letter, int seatNo) {
	int i;
	for (i = 0; i < ROWS * SEATS_PER_ROW; i++) {
		if (seatPointersArr[i] != NULL) {
			if (seatPointersArr[i]->letter == letter && seatPointersArr[i]->seatNo == seatNo) {
				printf("\nName: %s\nSeat: %c%d\n", seatPointersArr[i]->name, letter, seatNo);
				if (seatNo < 22) { printf("Level: First Class\n"); }
				else if (seatNo < 50) { printf("Level: Business Class\n"); }
				else if (seatNo < 57) { printf("Level: Priority Economy\n"); }
				else { printf("Level: Economy\n"); }
				return;
			}
		}
	}
	printf("\nBoarding pass not available!\n");
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
