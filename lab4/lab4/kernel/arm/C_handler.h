/*
Author: Satvik Dhandhania <sdhandha@andrew.cmu.edu> 
Vallari Mehta <vallarim@andrew.cmu.edu>
Date: 4th November 2015
*/
extern void C_SWI_handler(int swi_num, unsigned *regs);
extern void C_IRQ_handler();
extern void exit_handler(int status);
extern volatile unsigned long timer_count;
