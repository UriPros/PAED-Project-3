
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "graph.h"
#include "tree.h"
#include "table.h"


int StartMenu() {
    int option = 0;

    printf("\n._/\\ Super Structure Bros /\\_.\n");
    printf("\n1. Kart (Graphs)\n");
    printf("2. Brawl (Trees)\n");
    printf("3. Wonder (Tables)\n");
    printf("\n4. Stop\n");
    printf("\nPick a game: ");
    scanf("%d", &option);

    return option;
}

char StartMenuKart() {
    char kartOption;

    printf("\n-------------------\n");
    printf("MENU:\n");
    printf("\n\tA. Circuit detection\n");
    printf("\tB. Vehicle optimization\n");
    printf("\n\tC. Go back\n");
    printf("\nWhich tool do you want to use? ");
    scanf(" %c", &kartOption);


    return kartOption;
}

char StartMenuBrawl() {
    char brawlOption;

    printf("\n-------------------\n");
    printf("MENU:\n");
    printf("\n\tA. Add fighter\n");
    printf("\tB. Remove fighter\n");
    printf("\tC. Visual representation\n");
    printf("\tD. Search fighter\n");
    printf("\tE. Counterpick\n");
    printf("\n\tF. Go back\n");
    printf("\nWhich tool do you want to use? ");
    scanf(" %c", &brawlOption);

    return brawlOption;
}

char StartMenuWonder() {
    char wonderOption;

    printf("\n-------------------\n");
    printf("MENU:\n");
    printf("\n\tA. Add element\n");
    printf("\tB. Delete element\n");
    printf("\tC. Lookup\n");
    printf("\tD. Search by area\n");
    printf("\tE. Statistics\n");
    printf("\n\tF. Go back\n");
    printf("\nWhich tool do you want to use? ");
    scanf(" %c", &wonderOption);

    return wonderOption;
}

int main() {

    int option = 0;

    while (option != 4) {

        option = StartMenu();

        switch(option) {

            case 1: {

                char kartOption = 0;

                printf("\n-----------------------------------------\n\n");
                printf("YOU HAVE ENTERED TO SUPER STRUCTURE KART!\n\n");
                printf("-----------------------------------------\n");

                readGraphFile();

                while (kartOption != 'C') {

                    kartOption = StartMenuKart();

                    switch(kartOption) {

                        case 'A':
                            detectCircuits();
                            break;

                        case 'B':
                            vehicleOptimization();
                            break;

                        case 'C':
                            freeGraph();
                            break;
                    }
                }
            } break;

            case 2: {
                
                char brawlOption = 0;

                printf("\n------------------------------------------\n\n");
                printf("YOU HAVE ENTERED TO SUPER STRUCTURE BRAWL!\n\n");
                printf("------------------------------------------\n");

                int numFighters = readTreeFile();

                while (brawlOption != 'F') {

                    brawlOption = StartMenuBrawl();

                    switch(brawlOption) {

                        case 'A':
                            addFighter(numFighters);
                            break;

                        case 'B':
                            removeFighter();
                            break;

                        case 'C':
                            visualRepresentation(root);
                            break;

                        case 'D':
                            searchFighter(root);
                            break;

                        case 'E':
                            counterPick();
                            break;

                        case 'F':
                            break;
                    }
                }

            } break;
        
            case 3: {
                char wonderOption = 0;

                printf("\n-------------------------------------------\n\n");
                printf("YOU HAVE ENTERED TO SUPER STRUCTURE WONDER!\n\n");
                printf("-------------------------------------------\n");

                readTableFile();

                while (wonderOption != 'F') {

                    wonderOption = StartMenuWonder();

                    switch(wonderOption) {

                        case 'A':
                            // funció
                            add_element();
                            break; 

                        case 'B':
                            // funció
                            remove_element();
                            break;

                        case 'C':
                            // funció
                            lookup_element();
                            break;

                        case 'D':
                            // funció
                            search_by_area();
                            break;

                        case 'E':
                            // funció
                            // statistics
                        case 'F':
                            break;
                    }
                }
            } break;
                
            case 4: {
                printf("\nThank you for playing!\n");
                printf("\nTurning Super Structure Bros off.\n");
                printf("\n._/\\  ·  \\___/\\___/  ·  /\\_.\n\n");
                break;
            } break;
        }
    }


    return 0;
}
