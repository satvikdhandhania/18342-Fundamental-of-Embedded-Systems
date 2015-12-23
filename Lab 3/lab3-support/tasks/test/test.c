/** @file splat.c
 *
 * @brief Displays a prompt that goes into sleep for the amount of 
 * time the user specified.
 * @Author: Satvik Dhandhania <sdhandha@andrew.cmu.edu>
 * @        Vallari Mehta <vallarim@andrew.cmu.edu>
 * @ Date : 9th November 2015
 * Links to libc.
 */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define COUNTDOWN 5
#define INT_DIGITS 10        //length of digits for input
int main(int argc, char** argv)
{   
    int i=COUNTDOWN; 
    unsigned long wait_time;
    char buf[INT_DIGITS];
    printf("\nInitializing Alarm Clock:");
    while(i>0)
    {    
       printf("\n %d",i);
       sleep(1000);
       i--;
    }
    printf("\nCurrent time = %lums",time()*10); //converting to milliseconds
    printf("\nEnter time in seconds after which you want to wake up:");
    read(STDIN_FILENO,buf,INT_DIGITS);
    wait_time = atol(buf);
	wait_time = wait_time*1000;//converting to milliseconds
    printf("Current time = %lums",time()*10); //converting to milliseconds
    printf("\nGood Night!");
    sleep(wait_time);
	printf("\nCurrent time = %lums",time()*10); //converting to milliseconds
    printf("\nWAKE UP!\n");
    return 0;
}
