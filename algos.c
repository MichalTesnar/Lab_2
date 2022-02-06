/*
 * algos.c
 * 
 * Copyright 2022 Michal Tešnar <michal.tesnar007@gmail.com>
 */


#include <stdio.h>
#include <stdlib.h>

void printArray(int array[], int length){
	int index = 0;
	while(index != length){
		printf("%i ", array[index]);
		index++;
	}
	printf("\n");
}

void selectionSort(int array[], int length){
	int index = 0;
	int helper = 0;
	while(index != length){
		int min = index;
		while(helper != length){
			if(array[helper] < array[min]){
				min = helper;
			}
			helper++;
		}
		int saved = array[index];
		array[index] = array[min];
		array[min] = saved;
		
		index++;
		helper = index;
	}
}

void bubbleSort(int array[], int length){
	int counter = 0;
	while(counter != length){
		int helper = 0;
		while(helper != length - 1){
			if(array[helper]>array[helper+1]){
				int saved = array[helper+1];
				array[helper+1] = array[helper];
				array[helper] = saved;			
			}
			helper++;
		}
		counter++;
	}
	
}

int binarySearch(int array[], int length, int element){
	int left = 0;
	int right = length;
	while(array[left]!=element){
		
		int mid = (right + left)/2;
		
		if(array[mid] <= element){
			left = mid;
		} else {
			right = mid;
		}
	}
	return left;
}

int *dynamicIntArray(int sz){
	int *ptr = malloc(sz*sizeof(int));
	return ptr;
}

void copyArrayToArray(int array1[], int array2[], int start, int stop){
	for(int i = 0; i < stop-start; i++){
		array2[i] = array1[i+start];
	}
}

int *merge(int *array1, int length1, int *array2, int length2){
	int counter1 = 0;
	int counter2 = 0;
	int *ptr = malloc((length1+length2)*sizeof(int));
	for(int i = 0; i < length1+length2;i++){
		if(counter1 == length1){
			ptr[i] = array2[counter2];
			counter2++;
		} else if(counter2 == length2){
			ptr[i] = array1[counter1];
			counter1++;
		}else if(array1[counter1] < array2[counter2]){
			ptr[i] = array1[counter1];
			counter1++;
		}else{
			ptr[i] = array2[counter2];
			counter2++;
		}
		
	}
	return ptr;
}

void mergeSort(int *array, int length){
	if(length == 1){
		return;
	}
	int left = 0;
	int right = length;
	int mid = (left + right)/2;
	//printf("%i %i %i\n", left, right, mid);
	int *lArr = dynamicIntArray(mid);
	int *pArr = dynamicIntArray(right-mid);
	copyArrayToArray(array, lArr, 0, mid);
	copyArrayToArray(array, pArr, mid, right);
	//printArray(lArr, mid);
	mergeSort(lArr, mid);
	//printArray(pArr, right-mid);
	mergeSort(pArr, right-mid);
	
	int *finalArr = merge(lArr, mid, pArr, right-mid);
	
	copyArrayToArray(finalArr, array, 0, length);
}

int main(int argc, char **argv){
	
	int array[] = {9, 7, 11, 19, 22, 14, 46, 6, 13, 11, 8, 4, 29, 9, 3, 43, 14, 7, 15, 6, 23, 16, 16, 22, 32, 42, 26, 0, 44, 32, 0, 16, 3, 45, 17, 44, 24, 22, 24, 14, 30, 37, 25, 23, 25, 42, 27, 24, 41, 2, 36, 40, 28, 34, 42, 46, 19, 8, 9, 40, 20, 47, 48, 28, 25, 0, 34, 23, 2, 37, 4, 5, 45, 42, 46, 20, 23, 0, 15, 49, 37, 16, 49, 4, 27, 43, 44, 13, 46, 48, 12, 41, 22, 28, 13, 12, 22, 15, 23, 15};
	int length = 100;
	
	int aP[] = {1, 6, 9};
	int aS[] = {2, 3, 7};
	int *aQ;
	aQ = merge(aP, 3, aS, 3);
	printArray(aQ, 6);
	
	
	printArray(array, length);
	mergeSort(array, length);
	//copyArrayToArray(array, arr2, 0, 100);
	printArray(array, length);
	//printf("%i\n", binarySearch(array, length, 4));
	
	return 0;
}



