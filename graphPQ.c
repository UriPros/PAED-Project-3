#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "graph.h"
#include "graphPQ.h"


void swap(PQNode *a, PQNode *b) {
    PQNode tmp = *a;
    *a = *b;
    *b = tmp;
}

void PQ_insert(PrioQ *PQ, PQNode n) {

    PQ->items = realloc(PQ->items, (PQ->size + 1) * sizeof(PQNode));
    if (!PQ->items) {
        printf("Memory allocation error\n");
        exit(1);
    }

    int i = PQ->size;
    PQ->items[i] = n;
    PQ->size++;

    // Heapify up
    while (i > 0) {
        
        int parent = (i - 1) / 2;

        if (PQ->items[parent].dist <= PQ->items[i].dist) {
            break;
        }

        swap(&PQ->items[parent], &PQ->items[i]);
        i = parent;
    }
}

PQNode PQ_poll(PrioQ *PQ) {

    if (PQ->size == 0) {
        printf("Priority queue empty\n");
        exit(1);
    }

    PQNode min = PQ->items[0];

    PQ->items[0] = PQ->items[PQ->size - 1];
    PQ->size--;

    int i = 0;

    // Heapify down
    while (1) {
        int left = 2*i + 1;
        int right = 2*i + 2;
        int smallest = i;

        if (left < PQ->size && PQ->items[left].dist < PQ->items[smallest].dist) {
            smallest = left;
        }

        if (right < PQ->size && PQ->items[right].dist < PQ->items[smallest].dist) {
            smallest = right;
        }

        if (smallest == i) {
            break;
        }

        swap(&PQ->items[i], &PQ->items[smallest]);

        i = smallest;
    }

    return min;
}
