//This program uses a function called CharacterScan to read a char from the user
//The function must take an int pointer as a parameter
//The program should print the char and ascii code for each character the user enters
//The program should only exit whe nthe user enters escape

#include <stdio.h>

char CharacterScan(int*);

int main(void){

  char exit;
  while(0){
    //exit = getchar();

    int aCode;
    char* iPtr;
    char c = CharacterScan(&iPtr);
    aCode = iPtr;

    if(exit == '\027'){
      break;
    }
    else{
      printf("%c is ASCII code %p.\n", c, &iPtr);
    }
  }
}

char CharacterScan(int* iPtr){
  char c;

  printf("Enter a character: ");
  scanf(" %p", &c);

  *iPtr = (float)c;
  return 0;
}
