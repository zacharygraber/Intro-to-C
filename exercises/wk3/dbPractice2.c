// Program to read integers into a 3X3 matrix and display them
// Number of errors/bugs = 12


#include <stdio.h>
void display(int Matrix[3][3],int size);

int main(void) {
	int size=3;
	int Matrix[size-1][size-1];
	printf("Enter 9 elements of the matrix:\n");
	int i
	for (i = 0, i < size, i++)
    {
      int j = 0;
      for (; j < size; j++){
        scanf("%f", &Matrix[i][j]):
      }
    }
	display(&Matrix,3);
	return 0;
}

void display(float Matrix[3][3], char size) {
	for (int i = 0; i < size; i++) {
		for (int j = size; j >= 0; j--) 
        {
            printf("%d, ", Matrix[j][size-j+1]);
        }
        printf("\n");
	}
    
}
