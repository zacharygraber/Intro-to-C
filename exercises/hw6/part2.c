/******************************************************************
 * File: part2.c						  *
 * Description: solves FA1-20 CSCI-C 291 HW6 Part 2               *
 * Author: Zachary E Graber (zegraber@iu.edu)			  *
 * Date: 10/14/2020						  *
 ******************************************************************/

#include <stdio.h>
#include <string.h>

int numNumbers(FILE *fPtr);
int numCharsAndDigits(FILE *fPtr);
int numWordsWithVowel(FILE *fPtr);

int main(void) {
	char buffer[50], *fName;
	printf("Filename: ");
	scanf(" %49s", buffer);
	fName = (char*) malloc(sizeof(char) * strlen(buffer) + 1);
	strcpy(fName, buffer);

	FILE *fPtr = fopen("fName", "r");
	free(fName);
	if (fPtr == NULL) {
		printf("ERROR: COULD NOT OPEN FILE\n");
	}
	else {
		char mmInput;
		printf("\n\tMENU\n(a) Number of numbers\n(b) Number of chars and digits\n(c) Number of words with a vowel\n(d) Number of sentences\n(e) Analysis\n> ");
		scanf(" %c", &mmInput);
		switch (mmInput) {
			case 'a':
			case 'A':
				printf("Found %d numbers.\n", numNumbers(fPtr));
				break;
			case 'b':
			case 'B':
				printf("Found %d alphanumeric chars.\n", numCharsAndDigits(fPtr));
				break;
			case 'c':
			case 'C':
				break;
		}
		fclose(fPtr);
	}
	return(0);
}

int numNumbers(FILE *fPtr) {
	rewind(fPtr);
	int count = 0;
	float garbage;
	while (fscanf(fPtr, "%*[^0-9]%f", &garbage) == 1) { // Go until we hit a numeric value
		count += 1; // Increment the counter
	}
	return count;
}

int numCharsAndDigits(FILE *fPtr) {
	rewind(fPtr);
	int count = 0;
	char c;
	while (fscanf(fPtr, "%c", &c) == 1) {
		// If this char is alphanumeric, add one to the counter
		if ((48 <= (int)c && (int)c <= 57) || (65 <= (int)c && (int)c <= 90) || (97 <= (int)c && (int)c <= 122)) {
			count += 1;
		}
	}
	return count;
}

int numWordsWithVowel(FILE *fPtr) {
	rewind(fPtr);
	int count = 0;
	char word[51];
	// The definition of a word here is anything made up of alphanumeric characters
	while (fscanf(fPtr, "%[A-Za-z0-9]", word) == 1) {
		sprintf(word, "");i
	}
}
