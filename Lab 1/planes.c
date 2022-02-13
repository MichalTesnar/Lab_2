#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* name: planes.c
 *
 * author: Michal Tešnar (m.tesnar@student.rug.nl, s4740556)
 * author: Wojciech Trejter (w.trejter@student.rug.nl, s4296850)
 *
 * date 10.02.2022
 *
 * description: This program models an airport with a waiting runway
 * and a hangar for reparation. It takes input of planes landing and
 * whether they need to be repaired and prints the order in which
 * they leave.
 */

#include "LibStack.h"
#include "LibQueue.h"

void emptyRunway(int *runwayCounter, Queue *runway) {
    while (*runwayCounter != 0) {
        printf("%i\n", dequeue(runway));
        *runwayCounter -= 1;
    }
}

void emptyHangar(int *runwayCounter, Queue *runway, int *hangarCounter, Stack *hangar) {
    emptyRunway(runwayCounter, runway);
    while (*hangarCounter != 0) {
        enqueue(pop(hangar), runway);
        *hangarCounter -= 1;
        *runwayCounter += 1;
    }
}

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
    char repair[4];

    // procedure while planes are landing
    for (int i = 0; i < planes; i++) {

        // if runway is full then all planes leave on it leave the airport
        if (runwayCounter == 7) {
            emptyRunway(&runwayCounter, &runway);
        }

        // if hangar is full then all planes leave the runway, planes from hangar go to the runway
        if (hangarCounter == 5) {
            emptyHangar(&runwayCounter, &runway, &hangarCounter, &hangar);
        }

        // now neither hangar nor runway are full, we can add planes to them
        // get input line: identifier or the plain and whether it needs to be repaired
        scanf("%i %s", &planeNumber, repair);

        // if planes need to be repaired then it goes to the hangar
        if (repair[0] == 'y') {
            push(planeNumber, &hangar);
            hangarCounter++;
            // if that is not the case, we put them into the waiting runway
        } else {
            enqueue(planeNumber, &runway);
            runwayCounter++;
        }
    }

    // now all planes have landed
    // first we empty the hangar which also empties the runway
    emptyHangar(&runwayCounter, &runway, &hangarCounter, &hangar);
    // then we empty the runway
    emptyRunway(&runwayCounter, &runway);

    // free the memory of stacked hangar and runway
    freeStack(hangar);
    freeQueue(runway);
    return 0;
}