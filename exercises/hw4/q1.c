/*******************************************************
 * This program solves CSCI-C 291 Hw4 problem 1	       *
 * Author: Zachary E Graber (zegraber@iu.edu)	       *
 * Date: 9/23/2020				       *
 *******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUM_TOPICS 7
#define NUM_RESPONSES 10

int findIndexOfMaximum(int *array);

int main(void) {
	char mmInput;
	do {
		printf("\t\t\tMain Menu\n");
		printf("\t\t-------------------------\n");
		printf("\t\t(C) Create new Survey\n");
		printf("\t\t(Q) Quit\n");
		scanf(" %c", &mmInput);
		
		// Define and allocate our pointers (arrays) for topics and responses
		char **topics = (char**) malloc(NUM_TOPICS * sizeof(char*));
		int **responses = (int**) malloc(NUM_TOPICS * sizeof(int*));
		int i, j;
		for (i = 0; i < NUM_TOPICS; i++) {
			*(responses + i) = (int*) malloc(NUM_RESPONSES * sizeof(int)); // Make each pointer in responses an array capable of storing NUM_RESPONSES ints.
		}
		if (mmInput == 'C' || mmInput == 'c') {
			// Get input to initialize each topic
			char *topicInput = malloc(100 * sizeof(char));
			for (i = 0; i < NUM_TOPICS; i++) {
				printf("Enter topic %d: ", i+1);
				scanf(" %s", topicInput);
				*(topics + i) = (char*) malloc(((int) strlen(topicInput)) + 1);
				strcpy(*(topics + i), topicInput);
			}
			free(topicInput);
			
			// Get the inputs for each respondent
			int responseInput;
			for (i = 0; i < NUM_RESPONSES; i++) {
				printf("\n\t\t\tCollecting Input from Respondent %d\n", i+1);
				printf("\t\t--------------------------------------------------\n");
				for (j = 0; j < NUM_TOPICS; j++) {
					do {
						printf("From 1-10, please rate the importance of %s: ", *(topics + j));
						scanf(" %d", &responseInput);
					}
					while(!(responseInput > 0 && responseInput < 11));
					*(*(responses + j) + i) = responseInput;
				}
			}
			printf("\n\t\t\tStatistics\n");
			printf("\t\t--------------------------\n");
			int thisResponse, thisTotal, minIndex = 0, maxIndex = 0;
			int *totals = malloc(NUM_TOPICS * sizeof(int));
			for (i = 0; i < NUM_TOPICS; i++) {
				thisTotal = 0;
				printf("%-*.*s  ", 24, 24, *(topics + i));
				for (j = 0; j < NUM_RESPONSES; j++) {
					thisResponse = *(*(responses + i) + j);
					thisTotal += thisResponse;
					printf(" %2d ", thisResponse);
				}
				printf("\t  %2.1f\n", ((float) thisTotal) / NUM_RESPONSES); // Print the avg
				*(totals + i) = thisTotal;
				if (thisTotal > *(totals + maxIndex)) { maxIndex = i; } // If this is greater than the existing max
				if (thisTotal < *(totals + minIndex)) { minIndex = i; } // If this is the new min
			}
			printf("\nTopic with highest point total: %s (%d)\n", *(topics + maxIndex), *(totals + maxIndex));
			printf("Topic with the lowest point total: %s (%d)\n", *(topics + minIndex), *(totals + minIndex));
	
			// Implements a selection sort to make a sorted version of totals int array.
			int thisMaxIndex;
			printf("Topics in descending order of importance:\n");
			for (i = 0; i < NUM_TOPICS; i++) {
				thisMaxIndex = findIndexOfMaximum(totals);
				printf("%d: %s\n", i+1, *(topics + thisMaxIndex));
				*(totals + thisMaxIndex) = -1;
			}
			free(totals);
			printf("Total Number of Respondents: %d", NUM_RESPONSES);
			printf("\n");
		}

		// We want to free our topics, each pointer inside responses, and then responses itself
		for (i = 0; i < NUM_TOPICS; i++) {
			free(*(topics + i));
			free(*(responses + i));
		}
		free(topics);
		free(responses);
	}
	while(mmInput != 'Q' && mmInput != 'q');

	return(0);
}

int findIndexOfMaximum(int *array) {
	int max = -1; // Value -1 signifies that a real val hasn't been found yet
	int i;
	for (i = 0; i < NUM_TOPICS; i++) {
		if (max == -1 && *(array + i) != -1) {
			max = i;
		}
		else if (*(array + i) != -1 && *(array + i) > *(array + max)) {
			max = i;
		}
	}
	return max;
}
