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



// ----------------CREATE GRAPH FROM FILE----------------
void readfile() {
    FILE *f;

    f = fopen("graphData.txt", "r");

    if (!f) {
        printf("Error opening file!\n");
        return;
    }

    //1a part
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

        graph.checkpoints[id - 1].routes = NULL;
        graph.checkpoints[id - 1].numRoutes = 0;
        graph.checkpoints[id - 1].routesCapacity = 0;

        if (graph.checkpoints[id - 1].start == 1) {
            graph.num_circuits++;
        }
    }
    
    graph.start_indices = malloc(graph.num_circuits * sizeof(int));
    graph.end_indices   = malloc(graph.num_circuits * sizeof(int));       
    

    //2a part
    fscanf(f, "%d\n", &graph.totalRoutes);

    for (int i = 0; i < graph.totalRoutes; i++) {
        int originID, destID, length;
        char terrain[MAX_STRING];
        Checkpoint* route_origin;

        fscanf(f, "%d;%d;%d;%s\n", &originID, &destID, &length, terrain);

        //Assignar l'origen de la ruta al checkpoint corresponent
        route_origin = &graph.checkpoints[originID - 1];

        if (route_origin->routesCapacity == 0) {
            route_origin->routesCapacity = 2;
            route_origin->routes = malloc(route_origin->routesCapacity * sizeof(Route));

            if (!route_origin->routes) {
                printf("Memory allocation error\n");
                fclose(f);
                return;
            }
        }

        // Si capacity is full -> double it
        if (route_origin->numRoutes >= route_origin->routesCapacity) {
            route_origin->routesCapacity *= 2;
            route_origin->routes = realloc(route_origin->routes, route_origin->routesCapacity * sizeof(Route));

            if (!route_origin->routes) {
                printf("Memory allocation error\n");
                fclose(f);
                return;
            }
        }

        route_origin->routes[route_origin->numRoutes].origin = originID;
        route_origin->routes[route_origin->numRoutes].destination = destID;
        route_origin->routes[route_origin->numRoutes].length = length;
        strcpy(route_origin->routes[route_origin->numRoutes].terrain, terrain);

        route_origin->numRoutes++;
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

    // Free each checkpoint's routes
    for (int i = 0; i < graph.totalCheckpoints; i++) {
        free(graph.checkpoints[i].routes);
    }

    // Free the array of checkpoints
    free(graph.checkpoints);
    // Free start/end indices arrays
    free(graph.start_indices);
    free(graph.end_indices);

    // Optional: set pointers to NULL to avoid dangling pointers
    graph.checkpoints = NULL;
    graph.start_indices = NULL;
    graph.end_indices = NULL;
}


void printCircuit(Checkpoint start, Checkpoint end, int count, int circuits_found) {
    printf("Circuit %d)\n\n", circuits_found);
    printf("Start: %s (%d)\n", start.name, start.id);
    printf("End: %s (%d)\n", end.name, end.id);
    printf("Total: %d checkpoints\n\n", count);
}


// ----------------CIRCUIT DETECTION FUNCTION----------------
void DFS(Checkpoint *current, int *index_end, int visited[], int *count) {

    // Mark the current node as visited and increment count of checkpoints in circuit
    visited[current->id - 1] = 1;
    (*count)++;

    if (current->end == 1) {
        *index_end = current->id -1;
        return;
    }

    // Recur for all the vertices adjacent to this checkpoint
    for (int i = 0; i < current->numRoutes; i++) {

        int dest = current->routes[i].destination;

        if (!visited[dest - 1]) {
            DFS(&graph.checkpoints[dest - 1], index_end, visited, count);
        }
    }
}

void detectCircuits() {

    int circuits_found = 0;

    printf("The following %d circuits have been found:\n\n", graph.num_circuits);

    for (int i = 0; i < graph.totalCheckpoints; i++) {
        
        //heuristic per veure si tots els starts ja s'han trobat
        // protegir que el dataset estigui correcte
        if (circuits_found >= graph.num_circuits) {
            return;
        }

        if (graph.checkpoints[i].start == 1) {

            //Create variables needed for the circuit
            int checkpoints_in_circuit = 0;
            Checkpoint start;
            Checkpoint end;
            int index_end = graph.checkpoints[i].id;        //inicialitzar el end al start in case that there is only 1 node

            int visited[graph.totalCheckpoints];
            memset(visited, 0, graph.totalCheckpoints * sizeof(int));

            DFS(&graph.checkpoints[i], &index_end, visited, &checkpoints_in_circuit);

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



void print_saved_path(int path[], int pathLen, float cost[], int length[], char terrain[][MAX_STRING]) {
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



DijkstraResult Dijkstra(int start_index, int end_index, char vehicleType[]) {


    int numNodes = graph.totalCheckpoints;
    
    DijkstraResult result;
    PrioQ pq;
    PQNode startNode;
    float distance[numNodes];
    int previousNode[numNodes];
    int visited[numNodes];

    result.totalDist = MAX_FLOAT;
    result.pathLen   = 0;
    result.path = malloc(numNodes * sizeof(int));
    result.routeCost = malloc(numNodes * sizeof(float));
    result.routeLength = malloc(numNodes * sizeof(int));
    result.routeTerrain = malloc(numNodes * sizeof(*result.routeTerrain));

    startNode.id = start_index;
    startNode.dist = 0.0;

    pq.items = NULL;
    pq.size  = 0;

    for (int i = 0; i < numNodes; i++) {
        distance[i] = MAX_FLOAT;
        previousNode[i] = -1;
        visited[i] = 0;

        result.routeCost[i] = 0.0;
        result.routeLength[i] = 0;
        result.routeTerrain[i][0] = '\0';
    }

    distance[start_index] = 0.0;
    previousNode[start_index] = start_index;

    PQ_insert(&pq, startNode);

    while (pq.size > 0) {

        PQNode currentNode = PQ_poll(&pq);
        int current_index = currentNode.id;

        if (visited[current_index]) {
            continue;
        }

        visited[current_index] = 1;
        if (current_index == end_index) {
            break;
        }

        Checkpoint *currentCheckpoint = &graph.checkpoints[current_index];
        char *checkpointBoost = currentCheckpoint->boost;

        for (int i = 0; i < currentCheckpoint->numRoutes; i++) {

            Route *route = &currentCheckpoint->routes[i];
            int neighbor_index = route->destination - 1;

            if (visited[neighbor_index]) {
                continue;
            }

            int routeLength = route->length;
            char *terrain   = route->terrain;

            float baseCost = routeLength * terrainMultiplier(vehicleType, terrain);

            float finalCost = checkpoint_boost(baseCost, routeLength, checkpointBoost, terrain);

            float newDistance = distance[current_index] + finalCost;

            if (newDistance < distance[neighbor_index]) {

                distance[neighbor_index] = newDistance;
                previousNode[neighbor_index] = current_index;

                result.routeCost[neighbor_index] = finalCost;
                result.routeLength[neighbor_index] = routeLength;
                strcpy(result.routeTerrain[neighbor_index], terrain);

                PQ_insert(&pq, (PQNode){ neighbor_index, newDistance });                //falta canviar
            }
        }
    }

    result.totalDist = distance[end_index];

    if (result.totalDist < MAX_FLOAT) {

        int node_index = end_index;

        while (node_index != start_index) {
            result.path[result.pathLen] = node_index;
            result.pathLen++;

            node_index = previousNode[node_index];
        }

        result.path[result.pathLen] = start_index;
        result.pathLen++;
    }

    free(pq.items);

    return result;
}


void vehicleOptimization() {

    int option;
    char vehicleType[20];

    printf("\n");
    for (int i = 0; i < graph.num_circuits; i++) {
        printf("%d) %s (%d)\n",
               i + 1,
               graph.checkpoints[graph.start_indices[i]].name,
               graph.checkpoints[graph.start_indices[i]].id);
    }

    printf("\nPick a starting point: ");
    scanf("%d", &option);

    printf("\nEnter a type of vehicle (TERRESTRIAL, AQUATIC, AERIAL, LAVA) or ANY: ");
    scanf("%s", vehicleType);


    int selectedCircuit = option - 1;
    int start_index = graph.start_indices[selectedCircuit];
    int end_index   = graph.end_indices[selectedCircuit];

    /* ---------- ANY vehicle ---------- */
    if (strcmp(vehicleType, "ANY") == 0) {

        char vehicleTypes[][20] = {"TERRESTRIAL", "AQUATIC", "AERIAL", "LAVA"};
        int numVehicles = 4;

        DijkstraResult bestResult;
        bestResult.totalDist = MAX_FLOAT;
        char bestVehicle[20] = "";

        for (int i = 0; i < numVehicles; i++) {
            
            DijkstraResult currentResult = Dijkstra(start_index, end_index, vehicleTypes[i]);

            if (currentResult.totalDist < bestResult.totalDist) {

                if (bestResult.path != NULL) {
                    free(bestResult.path);
                    free(bestResult.routeCost);
                    free(bestResult.routeLength);
                    free(bestResult.routeTerrain);
                }

                bestResult = currentResult;
                strcpy(bestVehicle, vehicleTypes[i]);
            } 
            else {
                free(currentResult.path);
                free(currentResult.routeCost);
                free(currentResult.routeLength);
                free(currentResult.routeTerrain);
            }
        }

        printf("\nThe optimal vehicle type is %s.\n\n", bestVehicle);

        print_saved_path(bestResult.path, bestResult.pathLen, bestResult.routeCost, bestResult.routeLength, bestResult.routeTerrain);

        free(bestResult.path);
        free(bestResult.routeCost);
        free(bestResult.routeLength);
        free(bestResult.routeTerrain);
    }

    else {

        DijkstraResult result = Dijkstra(start_index, end_index, vehicleType);

        print_saved_path(result.path, result.pathLen, result.routeCost, result.routeLength, result.routeTerrain);

        free(result.path);
        free(result.routeCost);
        free(result.routeLength);
        free(result.routeTerrain);
    }
}


void freeDijkstraResult(DijkstraResult *r) {
    free(r->path);
    free(r->routeCost);
    free(r->routeLength);
    free(r->routeTerrain);
}

int main() {

    readfile();
    
    //printStructs(graph.checkpoints, graph.checkpoints->routes, graph.totalCheckpoints);

    detectCircuits();

    vehicleOptimization();

    freeGraph();    //revisar

    return 0;
}
