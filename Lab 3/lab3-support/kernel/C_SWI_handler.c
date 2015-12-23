/*
Author : Satvik Dhandhania <sdhandha@andrew.cmu.edu>
         Vallari Mehta <vallarim@andrew.cmu.edu>
Date : 24th Oct 2015
 */
#include "C_handler.h"
#include<bits/swi.h>
#include<bits/fileno.h>
#include<exports.h>
#include<bits/errno.h>
#include<arm/reg.h>
#include<arm/timer.h>

#define NULL_CHAR 0
#define END_OF_TRANSMISSION 4
#define BACKSPACE 8
#define DELETE 127
#define NEW_LINE 10
#define CARRAIGE_RETURN 13
#define START_SDRAM 0xa0000000
#define END_SDRAM 0xa3ffffff	
#define START_STRATA 0x00000000
#define END_STRATA 0x00ffffff

int WRITE(int fd,char *buf, int len);
int READ(int fd,char *buf, int BUF_LEN);
unsigned long GET_TIME();
void SLEEP(unsigned long millis);

void C_SWI_handler(int swi_num, unsigned *regs)
{

    switch(swi_num)
    {	//Memory mapped registers
        case READ_SWI: regs[0]=READ(regs[0],(char *)regs[1],regs[2]); 
                       break;

        case WRITE_SWI:regs[0]=WRITE(regs[0],(char *)regs[1],regs[2]);
                       break;

        case EXIT_SWI: exit_handler(regs[0]);
                       break;

        case TIME_SWI: regs[0] = GET_TIME();
                       break;

        case SLEEP_SWI: SLEEP(regs[0]);
                        break;

        default:puts("Invalid Instruction!! Exiting!");
                exit_handler(0xbadc0de); 
                break;
    }
}
unsigned long GET_TIME()
{

    return timer_count;

}    
void SLEEP(unsigned long millis)
{
    //Drift is added due division and memory accesses      
    unsigned long duration_in_ten_ms = millis/10;
    unsigned long end_time = timer_count + duration_in_ten_ms;
    while(timer_count < end_time)
        ;
}

int READ(int fd,char *buf, int BUF_LEN)
{
    int result=0;
    int count=0;
    char ch;
    if(fd==STDIN_FILENO)
    {
        while(count<BUF_LEN)
        {
            ch=(char) getc();
            if(ch==NULL_CHAR||ch==END_OF_TRANSMISSION)//Terminating 
                break;
            if(ch==BACKSPACE||ch==DELETE)//Handles backspace & DEL
            {
                if(count>0)
                {
                    count--;
                    buf[count]='\0';
                    puts("\b \b");
                }
                continue;
            }
            buf[count]=ch;
            count++;
            if(ch==NEW_LINE||ch==CARRAIGE_RETURN)
            {
                buf[count-1]='\n';
                putc('\n');	
                buf[count]='\0';
                break;
            }
            putc(ch);
        }
        result = count;
    }
    else
    {
        result = -EBADF;
    }//Checking for out of bounds memory access in read
    if(((unsigned) (buf+count)>=END_SDRAM)||((unsigned)buf< START_SDRAM))
    {
        result = -EFAULT;	
    }     

    return result;
}

int WRITE(int fd,char *buf, int len)
{
    int result=0;
    int count=0;
    char ch;
    int flag = 0;
	//Checking for out of bounds memory access in write
	if(((unsigned) (buf+count)<END_SDRAM)&&((unsigned)buf>=START_SDRAM))
	{
	  flag = 1;	
	}     
	if((unsigned) (buf+count)<END_STRATA)
	{
	  flag = 1;	
	}     
	if(!flag)
	   result = -EFAULT;
    if(fd==STDOUT_FILENO)
    {
        if(len>0)
        {
            while(count<len)
            {
                ch = buf[count];
                count++;
                putc(ch);
            }
		}
	       result = count;	
	}
	else
	{
		result = -EBADF;
	}

    return result;
}
