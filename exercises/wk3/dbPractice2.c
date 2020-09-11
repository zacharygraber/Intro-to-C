// Program to read integers into a 3X3 matrix and display them
// Number of errors/bugs = 12


#include <stdio.h>
void display(int matrix[3][3], int size);

int main(void) {
	int size = 3;
	int matrix[size][size];
	printf("Enter 9 elements of the matrix:\n");
	int i;
	for (i = 0; i < size; i++) {
        	int j;
      		for (j = 0; j < size; j++) {
        		scanf(" %d", &matrix[i][j]);
      		}
    	}
	display(matrix,3);
	return(0);
}

void display(int matrix[3][3], int size) {
	int i;
	for (i = 0; i < size; i++) {
		int j;
		for (j = 0; j < size; j++) {
            		printf("%d, ", matrix[i][j]);
        	}
        printf("\n");
	}
}
