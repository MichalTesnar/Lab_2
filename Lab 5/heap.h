/* name: heap.h
 *
 * author: Michal Tešnar (m.tesnar@student.rug.nl, s4740556)
 * author: Wojciech Trejter (w.trejter@student.rug.nl, s4296850)
 *
 * date: 23.03.2022
 * description: A header for a heap. It contains two arrays; one with weights, which 
 * the heap is sorted by. The other one are corresponding city numbers.
 */

#ifndef HEAP_H
#define HEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

typedef struct Heap {
  int *array; // weights of connections
  int *cityNumber; // identifiers of cities
  int front;
  int size;
} Heap;

void doubleHeapSize (Heap *hp);
int removeMin(Heap *hp);
void enqueue(int n, Heap *hp, int city);
void heapEmptyError();
int isEmptyHeap(Heap h);
Heap makeHeap();
void upheap(Heap *hp, int n);
void downheap(Heap *hp, int n);
void swap(int *a, int *b);
void freeHeap(Heap hp);
void printHeap(Heap hp);
void heapSort(int n, int arr[]);

#endif