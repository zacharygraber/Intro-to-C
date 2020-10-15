/************************************************************
 * File: part1.c					    *
 * Description: solves FA1-20 CSCI-C 291 HW6 Problem 1      *
 * Author: Zachary E. Graber (zegraber@iu.edu)		    *
 * Date: 10/14/2020					    *
 ************************************************************/

#include <stdio.h>
#include <string.h>
#define PATIENTS 20
int t=1, f=0;

struct patient {
	char lastName[15];
	char firstName[15];
	char gender;
	int* isCovidPositive;
};

typedef struct patient Patient;

void initializeBlank();
void getAndWriteInput();
void searchRecords(char lastName[15], char gender);
void printRecord(Patient *pPtr);

int main(void) {

	initializeBlank(); // Part a
	getAndWriteInput(); // Part b
	
	// Part c
	char lName[15], gender, input='s';
	while (input != 'q' && input != 'Q') {
		printf("\nSearch Records (s)\nQuit (q)\n> ");
		scanf(" %c", &input);
		if (input == 's' || input == 'S') {
			printf("Last Name ('unassigned' for none): ");
			scanf(" %14[^\n]", lName);
			printf("Gender ('n' for none): ");
			scanf(" %c", &gender);
			searchRecords(lName, gender);
		}
	}

	return(0);
}

// Part a
void initializeBlank() {
	FILE* fPtr = fopen("nameage.dat", "wb");
	if (fPtr == NULL) {
		printf("ERROR: COULD NOT OPEN/CREATE FILE FOR WRITING!\n");
	}
	else {
		int i;
		Patient p = {"unassigned", "", '\0', NULL};
		for (i = 0; i < PATIENTS; i++) {
			fwrite(&p, sizeof(p), 1, fPtr);
		}
		fclose(fPtr);
	}
}

// Part b
void getAndWriteInput() {
	FILE* fPtr = fopen("nameage.dat", "wb");
	if (fPtr == NULL) {
		printf("ERROR: COULD NOT OPEN FILE FOR WRITING\n");
	}
	else {
		int i;
		char covid_pos;
		Patient p;
		Patient *pPtr = &p;
		for (i = 0; i < PATIENTS; i += 2) {
			// Collect data
			printf("\nLast Name of Patient %d: ", i);
			scanf(" %14[^\n]", pPtr->lastName);
			printf("First Name of Patient %d: ", i);
			scanf(" %14[^\n]", pPtr->firstName);
			printf("Gender of Patient %d: ", i);
			scanf(" %c", &(pPtr->gender));
			printf("Is Patient %d Covid-Positive (y/n): ", i);
			do {
				scanf(" %c", &covid_pos);
			}
			while (!(covid_pos == 'y' || covid_pos == 'n'));
			pPtr->isCovidPositive = covid_pos == 'y' ? &t : &f; // Assign the appropriate address

			// Write in the appropriate entry
			if (i != 0)
				fseek(fPtr, sizeof(p), SEEK_CUR); // Skip to the next even numbered

			fwrite(pPtr, sizeof(p), 1, fPtr); // Write in the file
		}
		fclose(fPtr);
	}
}

// Part c
void searchRecords(char lastName[15], char gender) {
	FILE* fPtr = fopen("nameage.dat", "rb");
	int i;
	if (fPtr == NULL) {
		printf("ERROR: COULD NOT OPEN FILE FOR READING\n");
		return;
	}

	// No info provided
	if (!strcmp(lastName, "unassigned") && gender == 'n') {
		printf("\nNo info.\n\n");
		return;
	}
	// Search by gender only
	else if (!strcmp(lastName, "unassigned")) {
		Patient p;
		int matches = 0;
		for (i = 0; i < PATIENTS; i++) {
			fread(&p, sizeof(p), 1, fPtr);
			if (p.gender == gender) {
				matches += 1;
				printRecord(&p);
			}
		}
		if (matches == 0) { printf("No info.\n"); }
		else { printf("\nQuery matched %d results\n", matches); }
		return;
	}
	// Search by lastName only
	else if (gender == 'n') {
		Patient p;
		int matches = 0;
		for (i = 0; i < PATIENTS; i++) {
			fread(&p, sizeof(p), 1, fPtr);
			if (!strcmp(p.lastName, lastName)) {
				matches += 1;
				printRecord(&p);
			}
		}
		if (matches == 0) { printf("No info.\n"); }
		else { printf("\nQuery matched %d results\n", matches); }
		return;

	}
	// Search by both
	else {
		Patient p;
		int matches = 0;
		for (i = 0; i < PATIENTS; i++) {
			fread(&p, sizeof(p), 1, fPtr);
			if (p.gender == gender && strcmp(p.lastName, lastName)) {
				matches += 1;
				printRecord(&p);
			}
		}
		if (matches == 0) { printf("No info.\n"); }
		else { printf("\nQuery matched %d results\n", matches); }
		return;

	}
	fclose(fPtr);
}

// Part c helper
void printRecord(Patient *p) {
	printf("\nRecord for %s, %s:\nGender: %c\nCovid-Positive: %d\n\n", p->lastName, p->firstName, p->gender, *(p->isCovidPositive));
}
