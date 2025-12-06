#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tables.h"
#include "linkedlist.h"

void wonder_menu(ElementNode* table[]) {
    char tool;

    while (tool != 'F') {

        printf("\nA. Add element\n");
        printf("B. Remove element\n");
        printf("C. Lookup\n");
        printf("D. Search by area\n");
        printf("E. Statistics\n\n");
        printf("F. Go back\n\n");

        printf("Which tool do you want to use?\n");
        scanf(" %c", &tool);

        switch (tool) {
            case 'A':
                add_element(table);
                break;
            case 'B':
                remove_element(table);
                break;
            case 'C':
                lookup_element(table);
                break;
            case 'D':
                search_by_area(table);
                break;
            case 'E':
                // Code for tool E
                break;
            case 'F':
                // go back to main menu
                break;

        }

    }
}
