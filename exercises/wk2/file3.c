/*Debugging quiz - File No: 3 */
/* This program should perform the duties of a calculator 
    The program should ask for the input until 'q' is entered. 
    For each operation, print whether the result is >100 or <100 or equal to 100
    Finally, print the number of calculations done until 'q' is entered */

// Check for possible logical errors and rectify them 

// Total number of bugs: 20

#include<stdio.h>

int main(void){
  char input;
  int num1, num2, result, count;
  
  while (1) {
      printf("Welcome to the Calculator\nOperation choices:\tAddition(A)\n\t\t\tSubtraction(S)\n\t\t\tMultiplication(M)\n\t\t\tDivision(D)\n\t\t\tQuit(Q)\nEnter choice: ");

      scanf("%c", &input);

      if (input == 'A' || input == 'S' || input == 'M' || input == 'D') { // The case where valid input is given
          printf("Enter both numbers in required sequence: ");
          scanf("%d, %d", &num1, &num2);
          
          float result;
          switch (input) {
          case 'A': 
              result = num1 + num2;
              break;  
          case 'S': 
              result = num1 - num2;
              break;
          case 'M': 
              result = num1 * num2;
              break;
          case 'D':
              result = (float) num1 / num2;
              break;
          }

          if (result > 100) {
              printf("Greater than 100.\n");
          }
          else if (result == 100) {
              printf("Equal to 100.\n");
          }
          else if (result < 100) {
              printf("Less than 100.\n");
          }
          // Then increment the counter by 1, since 1 operation was performed.
          count += 1;
      }
      else if (input == 'Q' || input == 'q') { // Acts as our sentinel value.
          break;
      }
      else { // The case where valid input is not given; goes back to the top of the while() loop
          printf("Please choose a valid operation\n");
      }
  }
  printf("Number of operations performed: %d\n",count);
  return(0);
}
