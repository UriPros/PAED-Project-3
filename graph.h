#ifndef GRAPHS_H
#define GRAPHS_H

#define MAX_STRING 100
#define MAX_FLOAT 3.40282347e+38

// Basic route information
typedef struct {
    int origin;
    int destination;
    int length;
    char terrain[MAX_STRING];
} Route;

// Checkpoint (graph node)
typedef struct {
    int id;
    char name[MAX_STRING];
    char boost[MAX_STRING];
    int start;
    int end;

    Route* routes;   // adjacency list head
    int numRoutes;
    int routesCapacity;
} Checkpoint;

// Main graph
typedef struct {
    int totalCheckpoints;
    int totalRoutes;
    Checkpoint* checkpoints;
    int num_circuits;

    int* start_indices;
    int* end_indices; 
} Graph;

typedef struct {
    float totalDist;

    int *path;
    int pathLen;

    float *routeCost;
    int *routeLength;
    char (*routeTerrain)[MAX_STRING];
} DijkstraResult;


Graph graph; 

void readGraphFile();
void printCircuit(Checkpoint start, Checkpoint end, int count, int circuits_found);
void DFS(Checkpoint *current, int *index_end, int visited[], int *count);
void detectCircuits();
float terrainMultiplier(char* vehicle_type, char* terrain);
float checkpoint_boost(float base_cost, int length, char* boost, char* terrain);
void print_saved_path(int path[], int pathLen, float cost[], int length[], char terrain[][MAX_STRING]);
DijkstraResult Dijkstra(int start_index, int end_index, char vehicleType[]);
void vehicleOptimization();
void freeDijkstraResult(DijkstraResult *r);
void freeGraph();

#endif
