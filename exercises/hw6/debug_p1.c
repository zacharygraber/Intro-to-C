// Helps me debug part 1.
// Just prints all the entries in nameage.dat
#include <stdio.h>

typedef struct patient {
	char lastName[15];
	char firstName[15];
	char gender;
	int* isCovidPositive;
} Patient;

int main(void) {
	FILE* fPtr = fopen("nameage.dat", "rb");
	int i;
	Patient p;
	for (i = 0; i < 20; i++) {
		fread(&p, sizeof(Patient), 1, fPtr);
		printf("lastName: '%s', firstName: '%s', gender: %c, isCovidPositive: %p\n", p.lastName, p.firstName, p.gender, p.isCovidPositive);
	}
	fclose(fPtr);
	return(0);
}
