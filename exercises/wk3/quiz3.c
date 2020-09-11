/* Instructions:
 *
 *
 1. Write a program to assign a grade to Student. 
	Get the number of assignmnets from user.
  Get the students score out of how many points for each assignmnet.
	Calculate percentage and return the grade associated with a range of marks.
 2. The overall score range and the associated grades are as follows:
     a. [97-100] = A+
     b. [90-97] = A
     c. [80-90] = B
     d. [70-80] = C
     e. less than 70 is a failing grade and should be given an F
     e. greater than 100 or less than 0 is invalid and your code should print "Invalid"
 3. Use the skeleton code provided below to create a new function "printGrade" to assign a grade based on the score ranges.
 4. Use print statements in your function to display the grade. For example, if the input marks is 77, the output should read:
        "Your grade is : C"
 5. Please adhere to standard C syntax conventions and use comments to explain your code. Failure to do so will result in a penalty.
 *
 *
 * */
#include <stdio.h>
#include <math.h>

void printGrade(int score){
	//print the students grade based on their score in the class
	printf("Your grade is: ");
	if (score > 97) {
		printf("A+");	
	}
	else if (score >= 90) {
		printf("A");
	}
	else if (score >= 80) {
		printf("B");
	}
	else if (score >= 70) {
		printf("C");
	}
	else {
		printf("F");
	}
	printf("\n");
}

int main(void) {
	char ch;
	float score;
	int assignments;
	do {
		printf("How many assignmnets did you have ? ");
    		//get the number of assignments from the student
    		scanf(" %d", &assignments);

		printf("Enter your score for all assignments in the format '#/#': \n" );

		//get the student's score and the max score for each assignment
		int totalPointsPossible = 0;
		int thisScore;
		int thisMax;
		int i;
		for (i = 1; i <= assignments; i++) {
			printf("Score for assignment %d: ", i);
			scanf(" %d/%d", &thisScore, &thisMax);
			score += thisScore;
			totalPointsPossible += thisMax;
		}

		//calculate the student's percentage in the class using the information you've gathered.
		int percent = round(((float)score / (float)totalPointsPossible) * 100);

		printGrade(percent);

		printf(" \n Do you want to continue? (Y/N) ");
		scanf(" %c",&ch);
	} while(ch == 'y'|| ch == 'Y');
}
