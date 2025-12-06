#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tables.h"
#include "linkedlist.h"

//he fet aquesta pero si el carles ens penja una altra de model, la cambiem

int hash(char* name) {
    unsigned long hash = 5381;
    int c;

    while ((c = *name++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % MAX;
}

void add_element(ElementNode* table[]) {
    Element e;

    printf("\n\nElement name?");
    fgets(e.name, MAX, stdin);
    e.name[strlen(e.name) - 1] = '\0';

    printf("Element world?");
    fgets(e.world, MAX, stdin);
    e.world[strlen(e.world) - 1] = '\0';

    printf("Latitude?");
    scanf("%f", &e.latitude);

    printf("Longitude?");
    scanf("%f", &e.longitude);
    getchar();

    int index = hash(e.name);
    table[index] = table_add(table[index], e);

    printf("\n\n%s has been registered!\n\n", e.name);

}

void remove_element(ElementNode* table[]) {
    char name[MAX];
    printf("\n\nEnter the name of the production to delete: ");
    fgets(name, MAX, stdin);
    name[strlen(name) - 1] = '\0';

    int index = hash(name);
    Element* e = list_get(table[index], name);
    table[index] = list_remove(table[index], name);

    printf("\nThe element %s (%s - %.2f, %.2f) has been removed!\n\n", e->name, e->world, e->latitude, e->longitude);

}

void lookup_element(ElementNode* table[]) {
    char name[MAX];
    printf("\n\nEnter the name of the element to lookup: ");
    fgets(name, MAX, stdin);
    name[strlen(name) - 1] = '\0';

    int index = hash(name);
    Element* e = list_get(table[index], name);

    printf("\nThe following element has been found: %s (%s - %.2f, %.2f)\n\n", e->name, e->world, e->latitude, e->longitude);
}

void search_by_area(ElementNode* table[]) {
    char world[MAX];
    float lat1, lon1, lat2, lon2;

    printf("Enter a world name: ");
    printf("Enter the latitude for the first point: ");
    printf("Enter the longitude for the first point: ");
    printf("Enter the latitude for the second point: ");
    printf("Enter the longitude for the second point: ");

    printf("Elements found in the specified area:\n");
    for (int i = 0; i < MAX; i++) {
        ElementNode* current = table[i];
        while (current != NULL) {
            Element e = current->data;
            if (strcmp(e.world, world) == 0 &&
                e.latitude >= lat1 && e.latitude <= lat2 &&
                e.longitude >= lon1 && e.longitude <= lon2) {
                printf("%s (%s - %.2f, %.2f)\n", e.name, e.world, e.latitude, e.longitude);
            }
            current = current->next;
        }
    }
}

int main() {
    Element *e;
    FILE* fp;
    int num_elements, i;

    fp = fopen("tablesM.paed", "r");

    if (fp == NULL) {
        printf("Error opening file.\n");
    } else {
        fscanf(fp, "%d\n", &num_elements);
        e = malloc(num_elements * sizeof(Element));

        for (i = 0; i < num_elements; i++) {
            //guardar a estructura
        }
        fclose(fp);

        ElementNode* table[MAX];
        for(i = 0; i < MAX; i++) {
            table[i] = NULL;
        }

        for (i = 0; i < num_elements; i++) {
            int index = hash(e[i].name);
            table[index] = table_add(table[index], e[i]);
        }
    }
}
