#ifndef PQGRAPHS_H
#define PQGRAPHS_H

// --- Priority queue structure ---
typedef struct {
    int id;
    float dist; 
} PQNode;

typedef struct {
    PQNode* items;   // Array of nodes
    int size;      // Current number of nodes in the queue
} PrioQ;


void swap(PQNode *a, PQNode *b);
void PQ_insert(PrioQ *PQ, PQNode n);
PQNode PQ_poll(PrioQ *PQ);


#endif
