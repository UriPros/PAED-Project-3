#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_STRING 100
#define MAX_FLOAT 3.40282347e+38



//#include "PQgraph.h"      // Priority queue for branch & bound
// --- Priority queue structure ---
typedef struct {
    int id;
    float dist; 
} PQNode;

typedef struct {
    PQNode* items;   // Array of nodes
    int size;      // Current number of nodes in the queue
} PrioQ;

static void swap(PQNode *a, PQNode *b) {
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

        if (PQ->items[parent].dist <= PQ->items[i].dist)
            break;

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

        if (left < PQ->size &&
            PQ->items[left].dist < PQ->items[smallest].dist)
            smallest = left;

        if (right < PQ->size &&
            PQ->items[right].dist < PQ->items[smallest].dist)
            smallest = right;

        if (smallest == i)
            break;

        swap(&PQ->items[i], &PQ->items[smallest]);
        i = smallest;
    }

    return min;
}

PQNode PQ_peek(PrioQ *PQ) {
    if (PQ->size == 0) {
        printf("Priority queue empty\n");
        exit(1);
    }

    return PQ->items[0];
}


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

Graph graph; 

float globalBestDist;
int *globalBestPath;
int globalBestPathLen;

/* --- GLOBAL SHARED ROUTE ARRAYS (allocated after readfile knows size) --- */
float *route_cost_global;                     // route cost for each node (filled by Dijkstra)
int   *route_length_global;                   // route length for each node
char  (*route_terrain_global)[MAX_STRING];   // pointer to array of strings

/* --- BACKUP (best-so-far) ARRAYS for ANY implementation --- */
float *bestRouteCost;
int   *bestRouteLength;
char  (*bestRouteTerrain)[MAX_STRING];
int   *bestPathBackup;
int    bestPathLenBackup;
float  bestOverallDist;
char   bestVehicleTypeGlobal[20];



// ----------------CREATE GRAPH FROM FILE----------------
void readfile() {
    FILE *f;

    f = fopen("graphData.txt", "r");

    if (!f) {
        printf("Error opening file!\n");
        return;
    }

    fscanf(f, "%d\n", &graph.totalCheckpoints);

    graph.num_circuits = 0;


    graph.checkpoints = (Checkpoint*)malloc(graph.totalCheckpoints * sizeof(Checkpoint));


    for (int i = 0; i < graph.totalCheckpoints; i++) {
        int id;
        char name[MAX_STRING], boost[MAX_STRING];
        char startStr[10], endStr[10];

        fscanf(f, "%d;%[^;];%[^;];%[^;];%[^;\n]", &id, name, boost, startStr, endStr);

        graph.checkpoints[id - 1].id = id;
        strcpy(graph.checkpoints[id - 1].name, name);
        strcpy(graph.checkpoints[id - 1].boost, boost);
        graph.checkpoints[id - 1].start = strcmp(startStr, "True") == 0;
        graph.checkpoints[id - 1].end   = strcmp(endStr, "True") == 0;

        if (graph.checkpoints[id - 1].start == 1) {
            graph.num_circuits++;
        }

        graph.checkpoints[id - 1].routes = malloc(graph.totalCheckpoints * sizeof(Route)); //hem de canviar aixó per ferho eficient
        graph.checkpoints[id - 1].numRoutes = 0;


    }
    
    graph.start_indices = malloc(graph.num_circuits * sizeof(int));
    graph.end_indices   = malloc(graph.num_circuits * sizeof(int));       
        
    fscanf(f, "%d\n", &graph.totalRoutes);

        /* --- allocate global route arrays now that graph.totalCheckpoints is known --- */
    route_cost_global = malloc(graph.totalCheckpoints * sizeof(float));
    route_length_global = malloc(graph.totalCheckpoints * sizeof(int));
    route_terrain_global = malloc(graph.totalCheckpoints * sizeof(*route_terrain_global));

    /* --- allocate backups for ANY mode --- */
    bestRouteCost = malloc(graph.totalCheckpoints * sizeof(float));
    bestRouteLength = malloc(graph.totalCheckpoints * sizeof(int));
    bestRouteTerrain = malloc(graph.totalCheckpoints * sizeof(*bestRouteTerrain));
    bestPathBackup = malloc(graph.totalCheckpoints * sizeof(int));

    /* init bestOverallDist */
    bestOverallDist = MAX_FLOAT;

    
    for (int i = 0; i < graph.totalRoutes; i++) {
        int originID, destID, length;
        char terrain[MAX_STRING];

        fscanf(f, "%d;%d;%d;%s\n",
            &originID,
            &destID,
            &length,
            terrain
        );


        int index = originID - 1;
        int r = graph.checkpoints[index].numRoutes;

        graph.checkpoints[index].routes[r].origin = originID;
        graph.checkpoints[index].routes[r].destination = destID;
        graph.checkpoints[index].routes[r].length = length;
        strcpy(graph.checkpoints[index].routes[r].terrain, terrain);

        graph.checkpoints[index].numRoutes++;
        

    }
    fclose(f);

}

void printStructs(Checkpoint* checkpoints, Route* routes, int totalCheckpoints) {
    printf("\n---- Checkpoints ----\n");
    printf("Number of Checkpoints: %d\n", totalCheckpoints);
    for (int i = 0; i < totalCheckpoints; i++) {
        printf("Checkpoint ID: %d\n", checkpoints[i].id);
        printf("Name: %s\n", checkpoints[i].name);
        printf("Boost: %s\n", checkpoints[i].boost);
        printf("Start: %d\n", checkpoints[i].start);
        printf("End: %d\n", checkpoints[i].end);
        printf("\n");
    }
    printf("\n\n");

    printf("---- Routes ----\n");
    for (int i = 0; i < totalCheckpoints; i++) {
        Checkpoint cp = checkpoints[i];
        printf("Routes from Checkpoint ID %d:\n", cp.id);
        for (int j = 0; j < cp.numRoutes; j++) {
            Route route = cp.routes[j];
            printf("  To Checkpoint ID: %d\n", route.destination);
            printf("  Length: %d\n", route.length);
            printf("  Terrain: %s\n", route.terrain);
            printf("\n");
        }
        printf("\n");
    }

    printf("\n\n");

    printf("Print graph:\n");
    for (int i = 0; i < totalCheckpoints; i++) {
        Checkpoint cp = checkpoints[i];
        printf("Checkpoint ID %d (%s), (BOOST: %s):\n", cp.id, cp.name, cp.boost);
        for (int j = 0; j < cp.numRoutes; j++) {
            Route route = cp.routes[j];
            printf("  -> Checkpoint ID %d | Length: %d | Terrain: %s\n",
                   route.destination, route.length, route.terrain);
        }
        printf("\n");
    }
    printf("\nNum circutis: %d\n\n", graph.num_circuits);
    
}

void freeGraph() {
    for (int i = 0; i < graph.totalCheckpoints; i++) {
        free(graph.checkpoints[i].routes);
    }
    free(graph.checkpoints);

    /* free global arrays */
    free(route_cost_global);
    free(route_length_global);
    free(route_terrain_global);

    free(bestRouteCost);
    free(bestRouteLength);
    free(bestRouteTerrain);
    free(bestPathBackup);
}

void printCircuit(Checkpoint start, Checkpoint end, int count, int circuits_found) {
    printf("Circuit %d)\n\n", circuits_found);
    printf("Start: %s (%d)\n", start.name, start.id);
    printf("End: %s (%d)\n", end.name, end.id);
    printf("Total: %d checkpoints\n\n", count);
}


// ----------------CIRCUIT DETECTION FUNCTION----------------
void DFS(Graph graph, Checkpoint current, int *index_end, int visited[], int *count) {

    // Mark the current node as visited and increment count of checkpoints in circuit
    visited[current.id - 1] = 1;
    (*count)++;

    if (current.end == 1) {
        *index_end = current.id -1;
    }

    // Recur for all the vertices adjacent to this checkpoint
    for (int i = 0; i < current.numRoutes; i++) {

        int dest = current.routes[i].destination;

        if (!visited[dest - 1]) {
            DFS(graph, graph.checkpoints[dest - 1], index_end, visited, count);
        }
        /*else if (visited[dest - 1] && graph.checkpoints[dest - 1].end) {
            (*index_end) = dest - 1;
        }*/
    }
}



void detectCircuits() {

    int visited[graph.totalCheckpoints];
    memset(visited, 0, graph.totalCheckpoints * sizeof(int));
    int circuits_found = 0;

    printf("The following %d circuits have been found:\n\n", graph.num_circuits);

    for (int i = 0; i < graph.totalCheckpoints; i++) {
        
        //heuristic per veure si tots els starts ja s'han trobat
        // protegir que el dataset estigui correcte
        if (circuits_found >= graph.num_circuits) {
            return;
        }

        if (graph.checkpoints[i].start == 1 && !visited[graph.checkpoints[i].id - 1]) {

            //Create variables needed for the circuit
            int checkpoints_in_circuit = 0;
            Checkpoint start;
            Checkpoint end;
            int index_end = graph.checkpoints[i].id;        //inicialitzar el end al start in case that there is only 1 node

            DFS(graph, graph.checkpoints[i], &index_end, visited, &checkpoints_in_circuit);

            start = graph.checkpoints[i];
            end = graph.checkpoints[index_end];

            // Store circuit info
            graph.start_indices[circuits_found] = i;
            graph.end_indices[circuits_found] = index_end;

            circuits_found++;

            printCircuit(start, end, checkpoints_in_circuit, circuits_found);
        }
    }
}


// ---------------- VEHICLE OPTIMIZATION FUNCTION----------------
float terrainMultiplier(char* vehicle_type, char* terrain) {
    if (strcmp(terrain, vehicle_type) == 0) {
        return 1.0;
    }

    if (strcmp(terrain, "TERRESTRIAL") == 0) {
        return 1.0;
    }

    if (strcmp(terrain, "LAVA") == 0 && strcmp("LAVA", vehicle_type) != 0) {
        return 2.0;
    }

    if (strcmp(terrain, "CUT") == 0) {
        return 4.0;
    }

    return 1.5;  // default penalization
}



float checkpoint_boost(float base_cost, int length, char* boost, char* terrain) {

    if (strcmp(boost, "NONE") == 0)
        return base_cost;

    if (strcmp(boost, "SPEED") == 0)
        return base_cost * 0.75;

    if (strcmp(boost, "JUMP") == 0)
        return length * 1.0;   // ignore all penalties

    if (strcmp(boost, "ITEM") == 0) {
        if (strcmp(terrain, "CUT") == 0) {
            return base_cost;    // CUT cannot be ignored
        }
        else {
            return length * 1.0;     // ignore all other penalties
        }
    }

    return base_cost;
}



void print_saved_path(Graph graph, int path[], int pathLen, float cost[], int length[], char terrain[][MAX_STRING]) {
    float total = 0;
    printf("Here is the shortest path for this circuit:\n\n");

    for (int i = pathLen - 1; i >= 0; i--) {
        int idx = path[i];
        printf("%s (%d)\n",
               graph.checkpoints[idx].name,
               graph.checkpoints[idx].id);

        if (i > 0) {
            int next = path[i - 1];
            printf("::: +%.2f SRU (%s) :::\n",
                   cost[next], terrain[next]);
            total += cost[next];
        }
    }

    printf("\nTotal Effective Distance: %.2f SRU\n", total);
}





/*function dijkstra(G, S)
    for each vertex V in G
        distance[V] <- infinite
        previous[V] <- NULL
        If V != S, add V to Priority Queue Q
    distance[S] <- 0
	
    while Q IS NOT EMPTY
        U <- Extract MIN from Q
        for each unvisited neighbour V of U
            tempDistance <- distance[U] + edge_weight(U, V)
            if tempDistance < distance[V]
                distance[V] <- tempDistance
                previous[V] <- U
    return distance[], previous[]*/


void Dijkstra(Graph graph, int start_index, int end_index, char* vehicle_type) {

    globalBestPathLen = 0;

    //printf("\n\n\nDijkstra's algorithm called for vehicle type: %s from %s to %s\n\n", vehicle_type, graph.checkpoints[start_index].name, graph.checkpoints[end_index].name);

    float distance[graph.totalCheckpoints]; // array to hold the shortest distance from start to each checkpoint
    int paths[graph.totalCheckpoints];
    int visited[graph.totalCheckpoints];


    for (int i = 0; i < graph.totalCheckpoints; i++) {
        distance[i] = INFINITY;
        visited[i] = 0;
        paths[i] = -1;

        route_cost_global[i] = 0.0f;
        route_length_global[i] = 0;
        route_terrain_global[i][0] = '\0'; 
    }

    distance[start_index] = 0.0;
    paths[start_index] = start_index;

    int current = start_index;

    char boost[MAX_STRING];


    while (current != -1 && current != end_index) {

        Checkpoint current_checkpoint = graph.checkpoints[current];
        strcpy(boost, current_checkpoint.boost);       // Update the boost of the current checkpoint


        for (int i = 0; i < current_checkpoint.numRoutes; i++) {
            int dest = current_checkpoint.routes[i].destination - 1; //destination id to index

            if (!visited[dest]) {
                //float cost = current_checkpoint.routes[i].length * terrainMultiplier(vehicle_type, current_checkpoint.routes[i].terrain);
                //float total_cost = checkpoint_boost(cost, boost, current_checkpoint.routes[i].terrain);
                int length = current_checkpoint.routes[i].length;
                char* terrain = current_checkpoint.routes[i].terrain;

                float base_cost = length * terrainMultiplier(vehicle_type, terrain);
                float total_cost = checkpoint_boost(base_cost, length, boost, terrain);


                float new_dist = distance[current] + total_cost;

                if (new_dist < distance[dest]) {
                    distance[dest] = new_dist;
                    paths[dest] = current;

                    route_cost_global[dest] = total_cost;
                    route_length_global[dest] = length;
                    strcpy(route_terrain_global[dest], terrain);

                }
            }
        }

        visited[current] = 1;
        

        // Pick next unvisited node with minimum distance
        float bestDist = MAX_FLOAT;
        int best_index = -1;
                
        for (int i = 0; i < graph.totalCheckpoints; i++) {
            if (!visited[i] && distance[i] < bestDist) {
                bestDist = distance[i];
                best_index = i;
            }
        }

        current = best_index;
            
    }

    globalBestDist = distance[end_index];
        globalBestPathLen = 0;

        int cur = end_index;
        while (cur != start_index) {
            globalBestPath[globalBestPathLen++] = cur;
            cur = paths[cur];
        }
        
        globalBestPath[globalBestPathLen++] = start_index;


    //rebuild_path(paths, start_index, end_index, graph, route_cost_global, route_length_global, route_terrain_global);

}

void vehicleOptimization(){

    int option = 0;
    char vehicle_type[20];

    int visited[graph.totalCheckpoints];
    memset(visited, 0, sizeof(visited));

    printf("\n");
    for (int i = 0; i < graph.num_circuits; i++) {
        printf("%d) %s (%d)\n", i+1, graph.checkpoints[graph.start_indices[i]].name, graph.checkpoints[graph.start_indices[i]].id);
    }
    printf("\nPick a starting point: ");
    scanf("%d", &option);

    printf("\nEnter a type of vehicle (TERRESTRIAL, AQUATIC, AERIAL or LAVA) or ANY: ");
    scanf("%s", vehicle_type);
    //vehicle_type[strlen(vehicle_type)] = '\0';

    int selected_circuit = option - 1;
    globalBestPath = (int*) malloc(graph.totalCheckpoints * sizeof(int));

        if (strcmp(vehicle_type, "ANY") == 0) {
            const char* types[] = {"TERRESTRIAL", "AQUATIC", "AERIAL", "LAVA"};
            int num = 4;

            bestOverallDist = MAX_FLOAT;

            for (int v = 0; v < num; v++) {
                Dijkstra(graph, graph.start_indices[selected_circuit], graph.end_indices[selected_circuit], (char*)types[v]);

                if (globalBestDist < bestOverallDist) {
                    bestOverallDist = globalBestDist;
                    strcpy(bestVehicleTypeGlobal, types[v]);

                    bestPathLenBackup = globalBestPathLen;
                    for (int k = 0; k < globalBestPathLen; k++)
                        bestPathBackup[k] = globalBestPath[k];

                    for (int k = 0; k < graph.totalCheckpoints; k++) {
                        bestRouteCost[k] = route_cost_global[k];
                        bestRouteLength[k] = route_length_global[k];
                        strcpy(bestRouteTerrain[k], route_terrain_global[k]);
                    }
                }
            }

        printf("\nThe optimal vehicle type is %s.\n\n", bestVehicleTypeGlobal);

        print_saved_path(graph, bestPathBackup, bestPathLenBackup, bestRouteCost, bestRouteLength, bestRouteTerrain);

        return;
    }

    else {
        Dijkstra(graph, graph.start_indices[selected_circuit], graph.end_indices[selected_circuit], vehicle_type);

        print_saved_path(graph, globalBestPath, globalBestPathLen, route_cost_global, route_length_global, route_terrain_global);


    }


}

int main() {

    readfile();
    
    //printStructs(graph.checkpoints, graph.checkpoints->routes, graph.totalCheckpoints);

    detectCircuits();

    vehicleOptimization();

    freeGraph();

    return 0;
}
