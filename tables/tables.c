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

    // Code to add an element
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
    table[index] = list_remove(table[index], name);

    printf("\nThe element %s (e.world - e.latitude, e.longitude) has been removed!.\n\n", name);  //mal fet no estic agfant correctament les dades
    //potser puc fer un bucle comparant el nom dins de la llista abans de borrar i agafar les dades


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