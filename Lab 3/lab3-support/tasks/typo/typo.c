/** @file typo.c
 *
 * @brief Echos characters back with timing data.
 * @Author : Satvik Dhandhania <sdhandha@andrew.cmu.edu>
 * @         Vallari Mehta <vallarim@andrew.cmu.edu>
 * @ Date:  9th November 2015
 * Links to libc.
 */
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<unistd.h>

#define MAX_BUFFER_LENGTH 80
int main(int argc, char** argv)
{
	/* Add code here -- put your names at the top. */
    char buf[MAX_BUFFER_LENGTH];
    char prompt[3] = "\n>";
    int  len = strlen(prompt)+1;
    unsigned long absolute, decimal;
    unsigned long start_time, end_time, difference;
    while(1)
    {
      memset(buf,'\0',strlen(buf));
      printf("%s",prompt);
      start_time = time();  
      len = read(STDIN_FILENO, buf,MAX_BUFFER_LENGTH);
      end_time = time();
      difference = (end_time - start_time)/10;
      decimal = difference % 10;  
      absolute= difference / 10;
      printf("%s",buf);
      printf("%lu.%lus",absolute,decimal);
    }
    return 0;
}
