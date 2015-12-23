/*
 Satvik Dhandhania
 andrew id : sdhandha@andrew.cmu.edu
*/
#include "math.h"

int add(int x, int y)
{
 return x+y;
}
int sub(int x, int y)
{
 return x-y;
}
int mul(int x, int y)
{
 return x*y;
}
int cdiv(int x, int y)
{
 if(y == 0) 	// Divide by zero exception
   return 0;
 return x/y;
}
int mod(int x, int y)
{
 if (y == 0 ) // Mod by 0 is undefined
   return 0;
 return x%y;
}
