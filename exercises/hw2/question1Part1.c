/*------------------------------------------------------|
| Implements Question 1 Part 1 for FA20 C291 HW2        |
| Author: Zachary E Graber (zegraber@iu.edu)            |
| Date: 9/9/2020				        |
|_______________________________________________________*/
#include <stdio.h>
#include <stdbool.h>
int main() {
	bool sentinelValue = true;
	int weeklyNo, hourlyNo, commissionNo;
	int maleNo[3], femaleNo[3], domesticNo[3], internationalNo[3];
	char mainMenuInput;
	double avgSal, avgSalWeekly, avgSalHourly, avgSalCommission, avgSalMale, avgSalFemale, avgSalDomestic, avgSalInternational;
	const double COMMISSION_HOURLY_RATE = 7.1;

	while (sentinelValue) {
	    printf("\n\t\t\tMain Menu\n\t\t\t---------\n\t\t\tEnter payroll info (E)\n\t\t\tQuit (Q)\n");
	    scanf(" %c", &mainMenuInput);
	    switch (mainMenuInput) {
		case 'E':
		    printf("Please enter the number of workers of each type in the format W # H # C #\n");
		    scanf(" W %d H %d C %d", &weeklyNo, &hourlyNo, &commissionNo);
		    
		    int i;
		    char intOrDom, mOrF;
		    double thisSal;

		    // Go through all the weekly workers
		    for (i = 0; i < weeklyNo; i++) {
			printf("Enter info for weekly worker %d\n", i + 1);
			printf("Male or Female? (M/F): ");
			scanf("  %c", &mOrF);
			printf("International or Domestic? (I/D): ");
			scanf("  %c", &intOrDom);
			printf("Weeky Salary: ");
			scanf("  %lf", &thisSal);
			printf("\n");

			avgSalWeekly += thisSal;
			avgSal += thisSal;

			if (mOrF == 'M') {
			    maleNo[0] += 1;
			    avgSalMale += thisSal;
			}
			else if (mOrF == 'F') {
			    femaleNo[0] += 1;
			    avgSalFemale += thisSal;
			}

			if (intOrDom == 'I') {
			    internationalNo[0] += 1;
			    avgSalInternational += thisSal;
			}
			else if (intOrDom == 'D') {
			    domesticNo[0] += 1;
			    avgSalDomestic += thisSal;
			}
		    }
		    avgSalWeekly /= weeklyNo;

		    // Go through all the hourly workers
		    double hoursWorked, hourlyRate;
		    for (i = 0; i < hourlyNo; i++) {
			printf("Enter info for hourly worker %d\n", i + 1);
			printf("Male or Female? (M/F): ");
			scanf("  %c", &mOrF);
			printf("International or Domestic? (I/D): ");
			scanf("  %c", &intOrDom);
			printf("Number of hours worked this week: ");
			scanf("  %lf", &hoursWorked);
			printf("Hourly rate: ");
			scanf("  %lf", &hourlyRate);
			printf("\n");
			
			if (hoursWorked <= 10) {
			    thisSal = hoursWorked * hourlyRate;
			}
			else {
			    thisSal = (10 * hourlyRate) + ((hoursWorked - 10) * (hourlyRate * 1.5));
			}
			avgSalHourly += thisSal;
			avgSal += thisSal;

			if (mOrF == 'M') {
			    maleNo[1] += 1;
			    avgSalMale += thisSal;
			}
			else if (mOrF == 'F') {
			    femaleNo[1] += 1;
			    avgSalFemale += thisSal;
			}

			if (intOrDom == 'I') {
			    internationalNo[1] += 1;
			    avgSalInternational += thisSal;
			}
			else if (intOrDom == 'D') {
			    domesticNo[1] += 1;
			    avgSalDomestic += thisSal;
			}
		    }
		    avgSalHourly /= hourlyNo;

		    // Go through commissioned workers
		    for (i = 0; i < commissionNo; i++) {
			printf("Enter info for commission worker %d\n", i + 1);
			printf("Male or Female? (M/F): ");
			scanf("  %c", &mOrF);
			printf("International or Domestic? (I/D): ");
			scanf("  %c", &intOrDom);
			printf("Number of hours worked this week: ");
			scanf("  %lf", &hoursWorked);
			printf("\n");
			
			thisSal = 250 + (COMMISSION_HOURLY_RATE * hoursWorked);
			avgSalCommission += thisSal;
			avgSal += thisSal;
			
			if (mOrF == 'M') {
		    	    maleNo[2] += 1;
		    	    avgSalMale += thisSal;
			}
		    	else if (mOrF == 'F') {
		    	    femaleNo[2] += 1;
		    	    avgSalFemale += thisSal;
			}

		    	if (intOrDom == 'I') {
		    	    internationalNo[2] += 1;
		    	    avgSalInternational += thisSal;
			}
		    	else if (intOrDom == 'D') {
		    	    domesticNo[2] += 1;
		    	    avgSalDomestic += thisSal;
			}
		    }
		    avgSalCommission /= commissionNo;
		    avgSal /= (weeklyNo + hourlyNo + commissionNo);
		    avgSalMale /= (maleNo[0] + maleNo[1] + maleNo[2]);
		    avgSalFemale /= (femaleNo[0] + femaleNo[1] + femaleNo[2]);
		    avgSalInternational /= (internationalNo[0] + internationalNo[1] + internationalNo[2]);
		    avgSalDomestic /= (domesticNo[0] + domesticNo[1] + domesticNo[2]);

		    printf("\n");
		    printf("\t\t\tStatistics\n");
		    printf("%d workers total (%d Male / %d Female, %d Domestic / %d International)\n", (weeklyNo + hourlyNo + commissionNo), (maleNo[0] + maleNo[1] + maleNo[2]), (femaleNo[0] + femaleNo[1] + femaleNo[2]), (domesticNo[0] + domesticNo[1] + domesticNo[2]), (internationalNo[0] + internationalNo[1] + internationalNo[2]));
		    printf("%d Weekly workers (%d Male / %d Female, %d Domestic / %d International)\n", weeklyNo, maleNo[0], femaleNo[0], domesticNo[0], internationalNo[0]);
		    printf("%d Hourly workers (%d Male / %d Female, %d Domestic / %d International)\n", hourlyNo, maleNo[1], femaleNo[1], domesticNo[1], internationalNo[1]);
		    printf("%d Commission workers (%d Male / %d Female, %d Domestic / %d International)\n", commissionNo, maleNo[2], femaleNo[2], domesticNo[2], internationalNo[2]);
		    printf("Overall average salary: %f\n", avgSal);
		    printf("Average salary Male vs. Female: %f vs. %f\n", avgSalMale, avgSalFemale);
		    printf("Average salary Domestic vs. International: %f vs. %f\n", avgSalDomestic, avgSalInternational);
		    printf("Average salary Weekly vs. Hourly vs. Commission: %f vs. %f vs. %f\n", avgSalWeekly, avgSalHourly, avgSalCommission);
		    printf("\n");

		    break;

		case 'Q':
		    sentinelValue = false;
		    break;
	    }
	}
	return(0);
}
