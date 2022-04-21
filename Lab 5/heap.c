/* name: heap.c
 *
 * author: Michal Tešnar (m.tesnar@student.rug.nl, s4740556)
 * author: Wojciech Trejter (w.trejter@student.rug.nl, s4296850)
 *
 * date: 23.03.2022
 * description: a structure of heap. The root is has a minimal value. The heap 
 * operates on connection times, but it returns the number of a city with  
 * the smallest value. 
 */

#include "heap.h"

void doubleHeapSize(Heap *hp) {
  // creating arrays of double size
  double *newArr = malloc((hp->size) * 2 * sizeof(double));
  int *newCityArr = malloc((hp->size) * 2 * sizeof(int));
  // copying the content to the doubled arrrays
  for (int i = 0; i < hp->size; i++) {
    newArr[i] = hp->array[i];
    newCityArr[i] = hp->cityNumber[i];
  }
  hp->size *= 2; // double parameter
  free(hp->array);
  free(hp->cityNumber);
  hp->array = newArr; // assign the new arrays
  hp->cityNumber = newCityArr;
}

void upheap(Heap *hp, int n) {
  int parent = n / 2;
  if (n > 1 && hp->array[n] < hp->array[parent]) { // if upheap is needed swap corresponding values
    doubleSwap(&(hp->array[n]), &(hp->array[parent]));
    intSwap(&(hp->cityNumber[n]), &(hp->cityNumber[parent]));
    upheap(hp, parent);
  }
}

void downheap(Heap *hp, int n) {
  int indexMax = n; // the node (position) we are downheaping
  if (hp->front < 2 * n + 1) { 
    return;
  }
  if (hp->array[n] > hp->array[2 * n]) {
    indexMax = 2 * n;
  }
  if (hp->front > 2 * n + 1 && hp->array[indexMax] > hp->array[2 * n + 1]) {
    indexMax = 2 * n + 1;
  }
  if (indexMax != n) { // we have not passed any conditions, we are at the right node
    doubleSwap(&(hp->array[n]), &(hp->array[indexMax]));
    intSwap(&(hp->cityNumber[n]), &(hp->cityNumber[indexMax]));
    downheap(hp, indexMax); // propagate further
  }
}

// initialising a heap
Heap makeHeap() {
  Heap h;
  h.array = malloc(1 * sizeof(double));
  assert(h.array != NULL);
  h.cityNumber = malloc(1 * sizeof(int));
  assert(h.cityNumber != NULL);
  h.front = 1;
  h.size = 1;
  return h;
}

int isEmptyHeap(Heap h) {
  return (h.front == 1);
}

void heapEmptyError() {
  printf("heap empty\n");
  abort();
}

// add new element to the heap
void enqueue(double n, Heap *hp, int city) {
  int fr = hp->front;
  if (fr == hp->size) {
    doubleHeapSize(hp);
  }
  hp->array[fr] = n;
  hp->cityNumber[fr] = city;
  upheap(hp, fr);
  hp->front = fr + 1;
}

// removes the smallest (top) value from the heap
int removeMin(Heap *hp) {
  int n;
  if (isEmptyHeap(*hp)) {
    heapEmptyError();
  }
  n = hp->cityNumber[1];
  hp->front--;
  hp->array[1] = hp->array[hp->front];
  hp->cityNumber[1] = hp->cityNumber[hp->front];
  downheap(hp, 1);
  return n;
}

void intSwap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

void doubleSwap(double *a, double *b) {
  double temp = *a;
  *a = *b;
  *b = temp;
}

void freeHeap(Heap hp) {
  free(hp.array);
  free(hp.cityNumber);
}

void printHeap(Heap hp) {
  for (int idx = 1; idx < hp.front; ++idx) {
    printf("%f ", hp.array[idx]);
  }
  printf("\n");
  for (int idx = 1; idx < hp.front; ++idx) {
    printf("%d ", hp.cityNumber[idx]);
  }
  printf("\n");
}