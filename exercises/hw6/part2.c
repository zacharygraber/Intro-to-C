/******************************************************************
 * File: part2.c						  *
 * Description: solves FA1-20 CSCI-C 291 HW6 Part 2               *
 * Author: Zachary E Graber (zegraber@iu.edu)			  *
 * Date: 10/14/2020						  *
 ******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int numNumbers(FILE *fPtr);
int numCharsAndDigits(FILE *fPtr);
int numWordsWithVowel(FILE *fPtr);
int numSentences(FILE *fPtr);
void printAndSaveAnalysis(FILE *fptr);

int main(void) {
	char buffer[50], *fName;
	printf("Filename: ");
	scanf(" %49s", buffer);
	fName = (char*) malloc(sizeof(char) * strlen(buffer) + 1);
	strcpy(fName, buffer);

	FILE *fPtr = fopen(fName, "r");
	free(fName);
	if (fPtr == NULL) {
		printf("FATAL ERROR: COULD NOT OPEN FILE\n");
	}
	else {
		char mmInput;
		while (mmInput != 'q' && mmInput != 'Q') {
			printf("\n\tMENU\n(a) Number of numbers\n(b) Number of chars and digits\n(c) Number of words with a vowel\n(d) Number of sentences\n(e) Analysis\n(q) Quit\n> ");
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
					printf("Found %d words with a vowel.\n", numWordsWithVowel(fPtr));
					break;
				case 'd':
				case 'D':
					printf("Found %d sentences.\n", numSentences(fPtr));
					break;
				case 'e':
				case 'E':
					printAndSaveAnalysis(fPtr);
					break;
			}
		}
		fclose(fPtr);
	}
	return(0);
}

int numNumbers(FILE *fPtr) {
	rewind(fPtr);
	int count = 0;
	float garbage;
	while (fscanf(fPtr, "%*[^0-9]%f", &garbage) == 1) { // Go until we stop hitting numeric values
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
	int count = 0, i;
	char word[51];
	bool breakCase;
	// The definition of a word here is anything made up of alphanumeric characters or an apostrophe
	while (fscanf(fPtr, "%[A-Za-z0-9']", word) == 1) {
		breakCase = false;
		// Check to see if it has a vowel
		for (i = 0; word[i] != '\0' && !breakCase; i++) {
			switch (word[i]) {
				case 'a':
				case 'e':
				case 'i':
				case 'o':
				case 'u':
				case 'y':
				case 'A':
				case 'E':
				case 'I':
				case 'O':
				case 'U':
				case 'Y':
					count += 1;
					breakCase = true;
					break;
			}
		}
		// Skip the junk after the word
		fscanf(fPtr, "%*[^A-Za-z0-9]");
	}
	return count;
}

int numSentences(FILE *fPtr) {
	rewind(fPtr);
	int count = 0, scanfReturn;
	char punctuation, trailing;
	while (fscanf(fPtr, "%*[^.?!]%c", &punctuation) == 1) { // A sentence is defined as a string ending with . ? or ! then either a space, newline, or EOF
		scanfReturn = fscanf(fPtr, "%[ \n]", &trailing);
		if (scanfReturn == EOF || scanfReturn == 1)
			count += 1;
	}
	return count;
}

void printAndSaveAnalysis(FILE *fPtr) {
	int nums = numNumbers(fPtr);
	int charsAndDigits = numCharsAndDigits(fPtr);
	int vowelWords = numWordsWithVowel(fPtr);
	int sentences = numSentences(fPtr);
	printf("\n%-25s%d\n", "Numbers: ", nums);
	printf("%-25s%d\n", "Alphanumeric Chars: ", charsAndDigits);
	printf("%-25s%d\n", "Words with a vowel: ", vowelWords);
	printf("%-25s%d\n", "Sentences: ", sentences);

	FILE *writePtr = fopen("analysis.txt", "w");
	if (writePtr == NULL) { printf("ERROR: COULD NOT OPEN FILE FOR WRITING"); }
	else {
		fprintf(writePtr, "%-25s%d\n", "Numbers: ", nums);
		fprintf(writePtr, "%-25s%d\n", "Alphanumeric Chars: ", charsAndDigits);
		fprintf(writePtr, "%-25s%d\n", "Words with a vowel: ", vowelWords);
		fprintf(writePtr, "%-25s%d\n", "Sentences: ", sentences);

		fclose(writePtr);
		printf("\nSucessfully wrote to file analysis.txt!\n\n");
	}
}
