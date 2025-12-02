#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_STRING 100

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


// ----------------CREATE GRAPH FROM FILE----------------
void readfile() {
    FILE *f;

    f = fopen("myData.txt", "r");

    if (!f) {
        printf("Error opening file!\n");
        return;
    }

    fscanf(f, "%d\n", &graph.totalCheckpoints);

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

        graph.checkpoints[id - 1].routes = malloc(graph.totalCheckpoints * sizeof(Route));
        graph.checkpoints[id - 1].numRoutes = 0;
    }        
    
    graph.start_indices = (int*)malloc(graph.num_circuits * sizeof(int));
    graph.end_indices = (int*)malloc(graph.num_circuits * sizeof(int));

    fscanf(f, "%d\n", &graph.totalRoutes);
    
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
    printf("\nNum circutis: %d\n\n\n", graph.num_circuits);
    
}

void freeGraph() {
    for (int i = 0; i < graph.totalCheckpoints; i++) {
        free(graph.checkpoints[i].routes);
    }
    free(graph.checkpoints);
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

    // Recur for all the vertices adjacent to this checkpoint
    for (int i = 0; i < current.numRoutes; i++) {

        int dest = current.routes[i].destination;

        if (!visited[dest - 1]) {
            DFS(graph, graph.checkpoints[dest - 1], index_end, visited, count);
        }
        else if (visited[dest - 1] && graph.checkpoints[dest - 1].end) {
            (*index_end) = dest - 1;
        }
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

            circuits_found++;

            //Create variables needed for the circuit
            int checkpoints_in_circuit = 0;
            Checkpoint start;
            Checkpoint end;
            int index_end = graph.checkpoints[i].id;        //inicialitzar el end al start in case that there is only 1 node

            DFS(graph, graph.checkpoints[i], &index_end, visited, &checkpoints_in_circuit);

            start = graph.checkpoints[i];
            end = graph.checkpoints[index_end];

            graph.start_indices[circuits_found - 1] = start.id - 1;
            graph.end_indices[circuits_found - 1] = end.id - 1;
            
            printCircuit(start, end, checkpoints_in_circuit, circuits_found);
        }
    }
}


void vehicleOptimization(){

    int option = 0;
    char vehicle_type[20];

    printf("\n");
    for (int i = 0; i < graph.num_circuits; i++) {
        printf("%d) %s (%d)\n", i+1, graph.checkpoints[graph.start_indices[i]].name, graph.checkpoints[graph.start_indices[i]].id);
    }
    printf("\nPick a starting point: ");
    //fscanf("%d", &option);

    printf("\nEnter a type of vehicle (TERRESTRIAL, AQUATIC, AERIAL or LAVA) or ANY: ");
    //fscanf("%s", vehicle_type);
    //vehicle_type[strlen(vehicle_type)] = '\0';


    for (int i = 0; i < graph.num_circuits; i++) {

        //Dijkstra(graph, graph.start_indices[i], graph.end_indices[i], visited);
        
    }

}

int main() {

    readfile();
    printStructs(graph.checkpoints, graph.checkpoints->routes, graph.totalCheckpoints);


    detectCircuits();

    vehicleOptimization();


    freeGraph();

    return 0;
}
