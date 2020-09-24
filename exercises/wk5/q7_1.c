//This program should read a string from the user and print it using a character pointer
//The program is setup to use pointer offset notation to get each character of the string

#include <stdio.h>
#include <string.h>

void main(void){
  
  //int size = 10;
 
  char s[20];
  int * cPtr;
  scanf(" %x", &s);
  cPtr = s;

  //int i=0;
  while(&cPtr != '\0'){
    printf("%p", cPtr--);
      }

  printf("\n");
  return 0
}
