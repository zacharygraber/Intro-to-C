/************************************************************************************
 * This program uses a left-shift operator to pack 8 chars into an unsigned long.   *
 * Solves FA20 CSCI-C 291 Assignment 5 part 3 (Bonus)				    *
 *										    *
 * Author: Zachary E. Graber (zegrabere@iu.edu)					    *
 * Date: 10/1/2020								    *
 ************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#define NUM_CHARS 8
#define BITS_PER_BYTE 8

char* charToBinary(char c);
unsigned long packCharacters(char chars[NUM_CHARS]);
char* unpackCharacters(unsigned long packedChars);

int main(void) {
	char inputs[NUM_CHARS];
	int i;
	
	// Get all the chars as inputs
	for (i = 0; i < NUM_CHARS; i++) {
		printf("Enter char %d:  ", i+1);
		scanf(" %c", inputs + i);
	}
	
	// Print all values before packing
	printf("\nChars before packing\n---------------------\n");
	for (i = 0; i < NUM_CHARS; i++) {
		printf("%c\t%3d (decimal)\t0b%s\n", inputs[i], (int)inputs[i], charToBinary(inputs[i]));
	}
	
	unsigned long packedChars = packCharacters(inputs);
	char *unpackedChars = unpackCharacters(packedChars);
	printf("\nChars after unpacking\n---------------------\n");	
	for (i = 0; i < NUM_CHARS; i++)	{
		printf("%c\t%3d (decimal)\t0b%s\n", unpackedChars[i], (int)unpackedChars[i], charToBinary(unpackedChars[i]));
	}
	free(unpackedChars);

	return(0);
}

char* charToBinary(char c) {
	char bits[BITS_PER_BYTE + 1]; // 8 bits plus null char
	bits[BITS_PER_BYTE] = '\0'; // Set the last char to a null char

	// Travel from right to left, assigning each bit accordingly
	int cInt = (int) c;
	int i;
	for (i = (BITS_PER_BYTE - 1); i >= 0; i--) {
		if (cInt % 2 == 0) {
			bits[i] = '0';
		}
		else {
			bits[i] = '1';
		}
		cInt /= 2;
	}
	return bits;
}

unsigned long packCharacters(char chars[NUM_CHARS]) {
	unsigned long result = chars[0];
	int i;
	for (i = 1; i < NUM_CHARS; i++) {
		result = result << BITS_PER_BYTE;
		result = result | chars[i];
	}
	return result;
}

char* unpackCharacters(unsigned long packedChars) {
	char *chars =  (char *) malloc(NUM_CHARS * sizeof(char));
	int i;
	for (i = 0; i < NUM_CHARS; i++) {
		chars[i] = (char)((packedChars << (BITS_PER_BYTE * i)) >> (BITS_PER_BYTE * (NUM_CHARS - 1)));
	}
	return chars;
}
