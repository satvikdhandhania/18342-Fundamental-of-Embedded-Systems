/** @file io.c
 *
 * @brief Kernel I/O syscall implementations
 *
 * @author Satvik Dhandhania <sdhandha@andrew.cmu.edu>
 *         Vallari Mehta <vallarim@andrew.cmu.edu>
 * @date   Sun, 2 Dec 2015 00:07:38 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-11-16
 */

#include <types.h>
#include <bits/errno.h>
#include <bits/fileno.h>
#include <arm/physmem.h>
#include <syscall.h>
#include <exports.h>
#include <kernel.h>

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


/* Read count bytes (or less) from fd into the buffer buf. */
int read_syscall(int fd,char *buf, size_t len)
{

    int result=0;
    size_t count=0;
    char ch;
    if(fd==STDIN_FILENO)
    {
        while(count<len)
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


/* Write count bytes to fd from the buffer buf. */
int write_syscall(int fd,char *buf, size_t len)
{

    int result=0;
    size_t count=0;
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
        return -EFAULT;
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
