/* A program that makes a simple airline booking system.   *
 * Solves FA20 CSCI-C 291 HW2 Part 2                       *
 * Author: Zachary E Graber (zegraber@iu.edu)              *
 * Date: 9/16/2020 					   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>

int main(void) {
	const int ROWS = 15;
	const int SEATS_PER_ROW = 7;

	char mmInput;
	printf("Type 'F' for First Class\n");
	printf("Type 'B' for Business Class\n");
	printf("Type 'Q' to quit\n");
	printf("Type anything else for Economy\n");
	scanf(" %c", &mmInput);
	
	switch(mmInput) {
		case 'F':
			//TODO
			break;
		case 'B':
			//TODO
			break;
		case 'Q':
			sentinel = false;
			break;
		default:
			//TODO
			break;
	}

	return(0);
}
