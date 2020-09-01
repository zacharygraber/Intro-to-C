/* 
 * Basic C Program to print out a greeting, then count down from 100.
 * Author: Zachary E. Graber (zegraber@iu.edu)
 *
 * Modified from a program intended to print out hello world by Adeel Bhutta
*/

#include <stdio.h>
#include <unistd.h>

int main(void) {
  printf("\" *****\"{Welcome to Fall2020 - 'Corona-affected' 8-Week session @ IU}, \\\\ Every week, please start early and ask for help! Also, let's try to stay safe! //\"***** \"\n");
  int countdown = 100;
  while (1) { // 1 simply expesses a true condition, making this an infinite loop.
    printf("%.3d .....\n", countdown);
    countdown = countdown - 1;
    sleep(1);
  }
  return(0); 
}


