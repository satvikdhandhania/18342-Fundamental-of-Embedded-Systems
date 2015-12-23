/*
	Satvik DHandhania sdhandha@andrew.cmu.edu
*/
#include <stdlib.h>
#include <stdio.h>

/**
 * Function: oddball
 * Description: Baseline implementation for finding the number that
 *   occurs only once in the array.
 * Arguments:
 *   arr - pointer to start of integer array
 *   len - number of elements in array
 * Return:
 *   The number that occurs only once
 */
#ifdef DEFAULT
int oddball(int *arr, int len) {
	int i, j;
	int foundInner;
	int result = 0;

	for (i = 0; i < len; i++) {
		foundInner = 0;
		for (j = 0; j < len; j++) {
			if (i == j) {
				continue;
			}
			if (arr[i] == arr[j]) {
				foundInner = 1;
			}
		}
		if (foundInner != 1) {
			result = arr[i];
		}
	}

	return result;
}
#endif

#ifdef OPTIMIZE1
int oddball(int *arr, int len) {
	int i,j,foundInner;
	for(i = len - 1;i >= 0; i-- ){				//Loop inversed to count down loop 
		foundInner =0;
		for(j = len -1; j >= 0; j--){			
			if(i == j){
				continue;
			}
			if(arr[i] == arr[j]){
				foundInner = 1;
				break;				//Insert break to exit if pair is found
			}
		}
		if(foundInner == 0){
			return arr[i];				//Insert return cause our task is accomplished
		}			
	}
 return 0;							//Can remove if input is always correct
}
#endif

#ifdef OPTIMIZE2
int oddball(int *arr, int len) {

	int i;
	if(len > 1){				//Check loop length 
	for (i = len-1; i > 0; i—=2) {		//Since we know elements are in pairs we can unroll the loop to reduce the looping to 1/2 the number of times
		arr[0] ^ = arr[i];		//XORing multiple values will give the unique value as each pair will cancel itself. The one without a pair will give be left as result
		arr[0] ^ = arr[i-1]
	}
	}
	return arr[0];				//Return the non duplicate element.


/*	//Alternate solution
	int i,temp,flag=2;			//Temp is swapping variable, flag is to repeat the for loop twice. 
	while(flag>0){				// loop to repear for loop just twice so that most elements get mapped in their index positions 
	for(i= len-1;i>=0; i--){
		if(i!=arr[i]){			// This condition removes the case when an element is compared to itself 
		if((arr[i] != arr[arr[i]])&& arr[i]!=0){	// If array element arr[i] is not zero and not equal to the value stored at its index. We swap it so that at least one of the value = index
			temp = arr[i];
			arr[i] = arr[arr[i]];
			arr[temp] = temp;
		}
		else if(arr[i]==arr[arr[i]]){		// If the index position value is equal to current array index value, we make both of them 0 to eliminate them from our list
			arr[arr[i]] = 0;
			arr[i] = 0;
		}
		}
	}
	flag--;						// decrementing so that while loop only runs twice
	}
	temp = (len+1)/2;				// Calculate the value of n i.e maximum input range
	for(i = len-1; i>temp; i--){			// Now we check for duplicate elements that might have been swapped out of range[(n+1)to(len-1)] and set both values to be 0 if an element is found 
	  if(arr[i]!=0){				
		  if(arr[i] == arr[arr[i]]){
			arr[arr[i]] = 0;
			}
		  else{
			arr[arr[i]] = arr[i];
			arr[i] = 0;
			}
		}
	}		
	for(i = temp; i >= 0;i--)			//finally we check for the non zero element in the range to get our non duplicate element. 
		 if(arr[i] != 0)
			return arr[i];
return 0;
*/



}
#endif

/**
 * Function: randGenerator
 * Description: Generate a random array that is in compliance with
 *   lab specification
 * Arguments:
 *   arr - pointer to start of integer array
 *   len - number of elements in array
 * Return:
 *   Unused, but feel free to utilize it.
 */

int randGenerator(int *arr, int len) {
	int i, j, r, rcount;
	for (i = 0; i < len; i++) {
		do {
			rcount = 0;
			r = rand()%(len/2 + 1) + 1;
			for (j = 0; j < i && rcount < 2; j++) {
				if (arr[j] == r) {
					rcount++;
				}
			}
		} while(rcount >= 2);
		arr[i] = r;
		printf("%d ", r);
	}
	printf("\nDone generating\n");
	return 0;
}




