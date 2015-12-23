/*
 * rot13.c: ROT13 cipher test application
 *
 * Authors: Satvik Dhandhania <sdhandha@andrew.cmu.edu>
 * Date:    10/11/2015  22:00:00
 *
 * First we include headers so that we can access our defined wrappers.
 * We then accept string from user using the read wrapper and store the
 * return value in len. If len is 0 or negative we exit the main function 
 * using appropriate exit status. If it is positive we perform the rot13 
 * cipher on the text. The we write the ciphered text on the Std output.
 * If an incomplete string(<len) is written we make another attempt to 
 * write it. Then we check for -1 return in which some error had occured.
 * If error has occured we exit with status 1, otherwise we simply repeat 
 * the whole process.
 */

#include <unistd.h>		// for read(), write()
#include <stdlib.h>		// for exit()
#define MAX_BUFLEN 100		// defining Maximum Buffer Length
int main(int argc, char *argv[]) {
	int i, len, errcheck=0;
	char buf[MAX_BUFLEN];
	if(argc < 2){		//Checking for no arguments
	    write(STDOUT_FILENO,"No arguments have been passed!\n",32);
	} 
	else{
	    for(i = 1; i < argc; i++){		// looping for each argument
	    	len = 0;
		while(argv[i][len]!= '\0')	// calculating length of each
	     	 len++;	
		write(STDOUT_FILENO, argv[i],len); //printing each argument
		write(STDOUT_FILENO, "\n",1);
	}}
	while(1){
		write(STDOUT_FILENO,"Enter text:",11);
		len = read(STDIN_FILENO, buf, MAX_BUFLEN);
		if(len == 0)
	 	   exit(0);
		else if(len == -1)
	   	   exit(1);
		for(i = len-1; i >= 0; i--){
		   if(((buf[i]<78)&&(buf[i]>64))||((buf[i]<110)&&(buf[i]>96)))
			buf[i] += 13;
		   else if(((buf[i]<123)&&(buf[i]>109))||((buf[i]<91)&&(buf[i]>77)))
			buf[i] -= 13;
		}	
		write(STDOUT_FILENO,"Cipher Text:",12);
		errcheck = write(STDOUT_FILENO,buf,len);
		if(errcheck < len){
	   	    write(STDOUT_FILENO,"\nFailed Attempt,Reattempting!\n",30);
		    write(STDOUT_FILENO, buf, len);
	 	 }
		if(errcheck == -1)
	       	    exit(1);	
	}
   return 0;
}
