// APDS – Project 3: Non-Linear Data Structures – Graphs
// Code by Sara Gibert, Oriol Pros, Jan Porcar and Francesc Mateu
// Group: APDS-P3-G10

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "graph.h"
#include "graphPQ.h"

// ----------------CREATE GRAPH FROM FILE----------------
void readGraphFile() {
    FILE *f;

    f = fopen("graphData.txt", "r");

    if (!f) {
        printf("Error opening file!\n");
        return;
    }

    // Read the total number of checkpoints
    fscanf(f, "%d\n", &graph.totalCheckpoints);

    graph.num_circuits = 0;

    graph.checkpoints = (Checkpoint*)malloc(graph.totalCheckpoints * sizeof(Checkpoint));

    // Read checkpoint data: id;name;boost;start;end
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

        // Count circuits based on start checkpoints
        if (graph.checkpoints[id - 1].start == 1) {
            graph.num_circuits++;
        }
    }

    graph.start_indices = malloc(graph.num_circuits * sizeof(int));
    graph.end_indices   = malloc(graph.num_circuits * sizeof(int)); 


    // Read the total number of routes
    fscanf(f, "%d\n", &graph.totalRoutes);

    // Read route data: originID;destID;length;terrain
    for (int i = 0; i < graph.totalRoutes; i++) {
        int originID, destID, length;
        char terrain[MAX_STRING];
        Checkpoint* route_origin;

        fscanf(f, "%d;%d;%d;%s\n", &originID, &destID, &length, terrain);

        // Assign the route to the origin checkpoint
        route_origin = &graph.checkpoints[originID - 1];

        // Initialize routes array if not already done
        if (route_origin->routesCapacity == 0) {
            route_origin->routesCapacity = 2;
            route_origin->routes = malloc(route_origin->routesCapacity * sizeof(Route));

            if (!route_origin->routes) {
                printf("Memory allocation error\n");
                fclose(f);
                return;
            }
        }

        // Double capacity if full
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


void printCircuit(Checkpoint start, Checkpoint end, int count, int circuits_found) {
    printf("\n  Circuit %d)\n\n", circuits_found);
    printf("Start: %s (%d)\n", start.name, start.id);
    printf("End:   %s (%d)\n", end.name, end.id);
    printf("Total: %d checkpoints\n\n", count);
}


// ----------------CIRCUIT DETECTION FUNCTIONS----------------
void DFS(Checkpoint *current, int *index_end, int visited[], int *count) {

    // Mark the current node as visited and increment count of checkpoints in circuit
    visited[current->id - 1] = 1;
    (*count)++;

    // If current is an end checkpoint, set it as the end of the circuit
    if (current->end == 1) {
        *index_end = current->id - 1;
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

    printf("\n\nThe following %d circuits have been found:\n\n", graph.num_circuits);

    // Iterate through all checkpoints to find starting points
    for (int i = 0; i < graph.totalCheckpoints; i++) {

        // Heuristic to see if all starts have already been found and protect dataset from being correctly read
        if (circuits_found >= graph.num_circuits) {
            return;
        }

        // If this checkpoint is a start, perform DFS to find the circuit
        if (graph.checkpoints[i].start == 1) {

            //Create variables needed for the circuit
            int checkpoints_in_circuit = 0;
            Checkpoint start;
            Checkpoint end;
            int index_end = graph.checkpoints[i].id;        //initialize end to start in case that there is only 1 node

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
    printf("\n");
}


// ---------------- VEHICLE OPTIMIZATION FUNCTIONS----------------
float terrainMultiplier(char* vehicle_type, char* terrain) {
    // If terrain matches vehicle type, no penalty
    if (strcmp(terrain, vehicle_type) == 0) {
        return 1.0;
    }

    // TERRESTRIAL is default, no penalty
    if (strcmp(terrain, "TERRESTRIAL") == 0) {
        return 1.0;
    }

    // LAVA penalizes non-LAVA vehicles
    if (strcmp(terrain, "LAVA") == 0 && strcmp("LAVA", vehicle_type) != 0) {
        return 2.0;
    }

    // CUT is highly penalized
    if (strcmp(terrain, "CUT") == 0) {
        return 4.0;
    }

    return 1.5;  // default penalization
}

float checkpoint_boost(float base_cost, int length, char* boost, char* terrain) {
    // No boost
    if (strcmp(boost, "NONE") == 0)
        return base_cost;

    // SPEED reduces cost by 25%
    if (strcmp(boost, "SPEED") == 0)
        return base_cost * 0.75;

    // JUMP ignores all penalties
    if (strcmp(boost, "JUMP") == 0)
        return length * 1.0;   // ignore all penalties

    // ITEM ignores penalties except CUT
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
    printf("\nHere is the shortest path for this circuit:\n\n");

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

    // Initialize result structure
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

    // Initialize distances and arrays
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

    // Main Dijkstra loop
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

        // Explore neighbors
        for (int i = 0; i < currentCheckpoint->numRoutes; i++) {

            Route *route = &currentCheckpoint->routes[i];
            int neighbor_index = route->destination - 1;

            if (visited[neighbor_index]) {
                continue;
            }

            int routeLength = route->length;
            char *terrain   = route->terrain;

            // Calculate base cost with terrain multiplier
            float baseCost = routeLength * terrainMultiplier(vehicleType, terrain);

            // Apply checkpoint boost
            float finalCost = checkpoint_boost(baseCost, routeLength, checkpointBoost, terrain);

            float newDistance = distance[current_index] + finalCost;

            // If better path found, update
            if (newDistance < distance[neighbor_index]) {

                distance[neighbor_index] = newDistance;
                previousNode[neighbor_index] = current_index;

                result.routeCost[neighbor_index] = finalCost;
                result.routeLength[neighbor_index] = routeLength;
                strcpy(result.routeTerrain[neighbor_index], terrain);

                PQNode newNode;
                newNode.id = neighbor_index;
                newNode.dist = newDistance;
                PQ_insert(&pq, newNode);
            }
        }
    }

    result.totalDist = distance[end_index];

    // Reconstruct path if reachable
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
        printf("%d) %s (%d)\n", i + 1, graph.checkpoints[graph.start_indices[i]].name, graph.checkpoints[graph.start_indices[i]].id);
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

        printf("\nThe optimal vehicle type is %s.\n", bestVehicle);

        print_saved_path(bestResult.path, bestResult.pathLen, bestResult.routeCost, bestResult.routeLength, bestResult.routeTerrain);

        free(bestResult.path);
        free(bestResult.routeCost);
        free(bestResult.routeLength);
        free(bestResult.routeTerrain);
    }

    /* ---------- Specific vehicle ---------- */
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

/*int main() {
    readGraphFile();

    //printStructs(graph.checkpoints, graph.checkpoints->routes, graph.totalCheckpoints);

    detectCircuits();

    vehicleOptimization();

    freeGraph();
    return 0;
}*/
