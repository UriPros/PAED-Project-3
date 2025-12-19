#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"
#include "tableLL.h"

int hash(char name[]) {
    unsigned long hash = 5381;
    int c;

    while ((c = *name++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % MAX;
}

void add_element() {
    Element e;

    printf("\n\nElement name? ");
    scanf(" %99[^\n]", e.name);

    printf("Element world? ");
    scanf(" %99[^\n]", e.world);

    printf("Latitude? ");
    scanf("%f", &e.latitude);

    printf("Longitude? ");
    scanf("%f", &e.longitude);
    getchar();

    int index = hash(e.name);
    table[index] = list_add(table[index], e);

    if (table[index] == NULL) {
        printf("\nError adding element.\n\n");
        return;
    }

    printf("\n\n%s has been registered!\n\n", e.name);

}

void remove_element() {
    char name[MAX];
    printf("\n\nEnter the name of the production to delete: ");
    scanf(" %99[^\n]", name);

    int index = hash(name);
    Element* e = list_get(table[index], name);

    if (e == NULL) {
        printf("\nElement not found.\n\n");
        return;
    }

    printf("\nThe element %s (%s - %.2f, %.2f) has been removed!\n\n", e->name, e->world, e->latitude, e->longitude);

    table[index] = list_remove(table[index], name);
}

void lookup_element() {
    char name[MAX];
    printf("\n\nEnter the name of the element to lookup: ");
    scanf(" %99[^\n]", name);

    int index = hash(name);
    Element* e = list_get(table[index], name);

    if (e == NULL) {
        printf("\nElement not found.\n\n");
        return;
    }

    printf("\nThe following element has been found: %s (%s - %.2f, %.2f)\n\n", e->name, e->world, e->latitude, e->longitude);
}

void search_by_area() {
    char world[MAX];
    float lat1, lon1, lat2, lon2;
    int found = 0;

    printf("\nEnter a world name: ");
    scanf(" %99[^\n]", world);
    printf("\nEnter the latitude for the first point: ");
    scanf("%f", &lat1);
    printf("\nEnter the longitude for the first point: ");
    scanf("%f", &lon1);
    printf("\nEnter the latitude for the second point: ");
    scanf("%f", &lat2);
    printf("\nEnter the longitude for the second point: ");
    scanf("%f", &lon2);

    printf("Elements found in the specified area:\n");


    for (int i = 0; i < MAX; i++) {
        ElementNode* current = table[i];
        while (current != NULL) {
            Element e = current->data;
            if (strcmp(e.world, world) == 0 &&
                e.latitude >= lat1 && e.latitude <= lat2 &&
                e.longitude >= lon1 && e.longitude <= lon2) {
                printf("%s (%s - %.2f, %.2f)\n", e.name, e.world, e.latitude, e.longitude);
                found = 1;
            }
            current = current->next;
        }
    }

    if (!found) {
        printf("No elements found in the specified area.\n");
    }
}

void statistics() {
    // Function to compute and display statistics about the elements
}

void readTableFile() {
    Element *e;
    FILE* fp;
    int num_elements, i;

    fp = fopen("tableData.txt", "r");

    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    } 

    fscanf(fp, "%d\n", &num_elements);
    e = malloc(num_elements * sizeof(Element));

    for (i = 0; i < num_elements; i++) {
        fscanf(fp, "%[^;];%[^;];%f;%f\n", e[i].name, e[i].world, &e[i].latitude, &e[i].longitude);
    }
    fclose(fp);

    for(i = 0; i < MAX; i++) {
        table[i] = NULL;
    }

    for (i = 0; i < num_elements; i++) {
        int index = hash(e[i].name);
        table[index] = list_add(table[index], e[i]);
    }
    
}
