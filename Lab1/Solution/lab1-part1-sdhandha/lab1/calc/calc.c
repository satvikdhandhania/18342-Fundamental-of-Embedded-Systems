/*
 Satvik Dhandhania
 andrew id : sdhandha@andrew.cmu.edu
*/
#include<stdio.h>
#include "math.h"
int main()
{
 int a,b;
 char ch;
 int flag=0;
 while(1)
 { 
   if(scanf(" %d%c%d", &a, &ch, &b) == 3)  
    switch(ch)
    {
	case '+': printf("%d\n\n",add(a,b)); 
		 break;
	case '-': printf("%d\n\n",sub(a,b)); 
		 break;
	case '*': printf("%d\n\n",mul(a,b)); 
		 break;
	case '/': printf("%d\n\n",cdiv(a,b)); 
		 break;
	case '%': printf("%d\n\n",mod(a,b)); 
		 break;
	default: flag = 1;
		 break;

    }
    else
      break;
    if(flag)
 	break;
 }
 return 0;
}	
