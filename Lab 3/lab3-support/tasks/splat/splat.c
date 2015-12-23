/** @file splat.c
 *
 * @brief Displays a spinning cursor.
 * @ Author: Satvik Dhandhania <sdhandha@andrew.cmu.edu>
 * @         Vallari Mehta <vallarim@andrew.cmu.edu>   
 * @ Date: 8th November 2015
 * Links to libc.
 */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc, char** argv)
{
	
    while(1)
    {
      printf("\b\b|");
      sleep(200);
      printf("\b\b/");
      sleep(200);
      printf("\b\b-");
      sleep(200);
      printf("\b\b\\");
      sleep(200);
    }    
    return 0;
}
