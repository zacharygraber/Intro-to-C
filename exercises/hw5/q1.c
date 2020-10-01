/************************************************************
 * This file contains solutions to part 1 of Assignment 5   *
 *   for CSCI-C 291 Fall 2020.				    *
 *							    *
 * Author: Zachary E Graber				    *
 * Date: 100/1/2020					    *
 ************************************************************/

#include <stdio.h>
#include <stdbool.h>

// Provided source code
struct patientinformation {
	char patient_name[20];
	char* abbreviated_name;
	unsigned int age;
	struct {
		char phone_number[11];
		char address[60];
		char city[20];
		char state[3];
		int zipCode;
	} physical_address;
	struct {
		bool istested;
		bool istested_positive;
	} covid_test;
} healthrecord, *healthrecord_ptr;
healthrecort_ptr = &healthrecord;

// Part A
healthrecord.patient_name

// Part B
healthrecord_ptr->patient_name

// Part C
healthrecord.abbreviated_name

// Part D
healthrecord_ptr->abbreviated_name

// Part E
char charArray[4]; // This will give enough room for 3 initials and '\0'
healthrecord.abbreviated_name = charArray;
printf("%s", healthrecord_ptr->abbreviated_name);

// Part F
healthrecord.physical_address.phone_number


// Part G
(healthrecord_ptr->physical_address).address

// Part H
healthrecord.covid_test.istested_positive

// Part I
(healthrecord_ptr->covid_test).istested

// Part J
struct patientinformation healthrecords[5];
int i;
for (i = 0; i < 5; i++) {
	printf("%d\n", healthrecords[i].physical_address.zipCode);
}

// Part K
struct patientinformation *arrayPtr = healthrecords;
for (i = 0; i < 5; i++) {
	(*(arrayPtr + i)).covid_test.istested = false;
}
