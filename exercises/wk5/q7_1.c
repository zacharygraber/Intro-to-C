//This program should read a string from the user and print it using a character pointer
//The program is setup to use pointer offset notation to get each character of the string

#include <stdio.h>
#include <string.h>

int main(void){
  
  //int size = 10;
 
  char s[20];
  char * cPtr;
  scanf(" %s", s);
  cPtr = s;

  //int i=0;
  while(*(cPtr) != '\0') {
    printf("%c", *(cPtr++));
  }

  printf("\n");
  return(0);
}
