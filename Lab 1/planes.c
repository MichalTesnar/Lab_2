#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* name: planes.c
 *
 * author: Michal Tešnar (m.tesnar@student.rug.nl, s4740556)
 * author: Wojciech Trejter (w.trejter@student.rug.nl, s4296850)
 *
 * date 09.02.2022
 *
 * description: This program models an airport with a waiting runway
 * and a hangar for reparation. It takes input of planes landing and
 * whether they need to be repaired and prints the order in which
 * they leave.
 */
 
#include "LibStack.h"
#include "LibQueue.h"

int main(int argc, char *argv[]) {
	// get the amount of landing planes
	int planes;
	scanf("%i", &planes);
	
	// define runway (queue) and hangar (stack) and counters to track amount of planes on them
	Queue runway = newQueue(8);
	int runwayCounter = 0;
	Stack hangar = newStack(6);
	int hangarCounter = 0;
	
	// define variables to hold input 
	int planeNumber;
	char repair[3];
  
	// procedure while planes are landing
	for(int i = 0; i < planes; i++){
		
		// if runway is full then all planes leave on it leave the airport
		if(runwayCounter == 7){
			while(runwayCounter != 0){
				printf("%i\n", dequeue(&runway));
				runwayCounter--;
			}
		}
		
		// if hangar is full then all planes leave the runway, planes from hangar go to the runway	
		if(hangarCounter == 5){
			while(runwayCounter != 0){
				printf("%i\n", dequeue(&runway));
				runwayCounter--;
			}
			while(hangarCounter != 0){
				enqueue(pop(&hangar), &runway);
				hangarCounter--;
				runwayCounter++;
			}
		}
		
		// now neither hangar nor runway are full, we can add planes to them
		// get input line: identifier or the plain and whether it needs to be repaired
		scanf("%i %s", &planeNumber, repair);
		
		// if planes need to be repaired then it goes to the hangar
		if(repair[0] == 'y'){
			push(planeNumber, &hangar);
			hangarCounter++;
		// if that is not the case, we put them into the waiting runway
		} else {
			enqueue(planeNumber, &runway);
			runwayCounter++;
		}
	}
	
	// now all planes have landed
	// first we empty the runway
	while(runwayCounter != 0){
		printf("%i\n", dequeue(&runway));
		runwayCounter--;
	}
	// then hangar empties onto runway
	while(hangarCounter != 0){
		enqueue(pop(&hangar), &runway);
		hangarCounter--;
		runwayCounter++;
	}
	// then we again empty the runway
	while(runwayCounter != 0){
		printf("%i\n", dequeue(&runway));
		runwayCounter--;
	}

	// free the memory of stacked hangar and runway
	freeStack(hangar);
	freeQueue(runway);
	return 0;
}
