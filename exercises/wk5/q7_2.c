//This program includes a struct called cat that is used to store information about a given cat
//The program should ask the user for information about their cat and initialize a cat structure with the given information
//The program should also print the information using a pointer to the initialized struct
//The information printed using the struct and the pointer should be the same

#include <stdio.h>
#include <string.h>
// make name a string

struct car{
	char* car;
	int age;
	float speed;
};

int main(void){
	char n[20];
	int a;
	float w;

	struct car c;
        struct car *cPtr = &c;


	printf("What is your favorite car's name: ");
	scanf(" %s", n);
	c.car = n;

	printf("How old is it:" );
	scanf(" %d", &a);
	c.age = a;

	printf("How much speed does it give: ");
	scanf(" %f", &w);
	c.speed = w;
        

	printf("Car's name is %s, and should be the same as %s.\n", c.car, cPtr->car);
	printf("Car's age is %d, and should be the same as %d.\n", c.age, cPtr->age);
	printf("Car's speed is %f, and should be the same as %f.\n", c.speed, cPtr->speed);
}
