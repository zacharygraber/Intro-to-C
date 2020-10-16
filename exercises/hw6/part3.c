/******************************************************************************
 * File: part3.c							      *
 * Description: solves part3 of CSCI-C 291 FA1-20 HW6.			      *
 * Author: Zachary E Graber (zegraber@iu.edu)                                 *
 * Date: 10/15/2020                                                           *
 ******************************************************************************/

#include <stdio.h>
#include <string.h>

char* clean_quotes(char* input_s);

// First Part
int main(void) {
	char john1[10], john2[10], john3[10];

	FILE *fPtr = fopen("john1.txt", "r");
	if (fPtr == NULL) { printf("Couldn't open john1.txt\n"); }
	else {
		fscanf(fPtr, "%s", john1);
		fclose(fPtr);
	}

	fPtr = fopen("john2.txt", "r");
	if (fPtr == NULL) { printf("Couldn't open john2.txt\n"); }
	else {
		fscanf(fPtr, "%s", john2);
		fclose(fPtr);
	}

	fPtr = fopen("john3.txt", "r");
	if (fPtr == NULL) { printf("Couldn't open john3.txt\n"); }
	else {
		fscanf(fPtr, "%s", john3);
		fclose(fPtr);
	}

	// The single and double quotes are read in as part of the string.
	// This makes sense, since the %s specifier just scans chars until it hits a whitespace
	printf("john: %s\n\"john\": %s\n'john': %s\n", john1, john2, john3);

	return (0);
}

// Second Part
// For any given input string, there is no possible way to tell the difference between a single quote and an apostrophe.
// As such, I interpreted it to mean "don't remove 's"
char* clean_quotes(char* input_s) {
	int input_s_len = strlen(input_s);
	char buffer[input_s_len + 1]; // We need at most the same space as the input.
	char thisChar;
	int w_index = 0, r_index; // The indices we're reading from and writing to.
	for (r_index = 0; r_index <= input_s_len; r_index++) {
		thisChar = *(input_s + r_index);
		if (thisChar == ''' && r_index < input_s_len - 1) { // only check for 's if it's not the last char or null char at the end
			if (*(input_s + r_index + 1) == 's') {
				buffer[w_index] = thisChar;
				w_index++;
			}
		}
		else if (thisChar != '"') {
			buffer[w_index] = thisChar;
			w_index++;
		}

	}

	// Make a new resulting string of the appropriate size and copy the contents of the buffer into it
	char *result = (char*) malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(result, buffer);
	return result;
}
