/*
 * Example how to use stack with back-end provided from uni.
 * 
 * Use "gcc example.c LibStack.c" + a.exe to compile! 
 *
 * Michal Tešnar <michal.tesnar007@gmail.com>
 */


#include <stdio.h>
#include "LibStack.h"
#include <string.h>

int match(char left, char right){
	return (left == '[' && right == ']') || (left == '(' && right == ')');
}

int main(int argc, char *argv[]) {
	Stack st = newStack(4);
	char input[100];
	scanf("%s", input);
	
	int length = strlen(input);
	
	for(int index = 0; index < length; index++){
		if(input[index] == '(' || input[index] == '['){
			push(input[index], &st);
		} else {
			if(isEmptyStack(st) || !match(pop(&st), input[index])){
				freeStack(st);
				printf("NOT GOOD\n");
			}
		}
	}
	
	if(isEmptyStack(st)){
		printf("GOOD\n");	
	}
	freeStack(st);
}

