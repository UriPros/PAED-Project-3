// APDS – Project 3: Non-Linear Data Structures – Main Program
// Code by Sara Gibert, Oriol Pros, Jan Porcar and Francesc Mateu
// Group: APDS-P3-G10

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "graph.h"
#include "tree.h"
#include "table.h"


int StartMenu() {
    // Display main menu and get user choice
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
    // Display kart (graph) submenu
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
    // Display brawl (tree) submenu
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
    // Display wonder (table) submenu
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
    // Main program loop
    int option = 0;

    while (option != 4) {

        option = StartMenu();

        switch(option) {

            case 1: {
                // Kart (Graphs) mode
                char kartOption = 0;

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
                // Brawl (Trees) mode
                char brawlOption = 0;

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
                // Wonder (Tables) mode
                char wonderOption = 0;

                readTableFile();

                while (wonderOption != 'F') {

                    wonderOption = StartMenuWonder();

                    switch(wonderOption) {

                        case 'A':
                            add_element();
                            break; 

                        case 'B':
                            remove_element();
                            break;

                        case 'C':
                            lookup_element();
                            break;

                        case 'D':
                            search_by_area();
                            break;

                        case 'E':
                            // statistics
                        case 'F':
                            break;
                    }
                }
            } break;
                
            case 4: {
                // Exit
                printf("\nThank you for playing!\n");
                printf("\nTurning Super Structure Bros off.\n");
                printf("\n._/\\  ·  \\___/\\___/  ·  /\\_.\n\n");
                break;
            } break;
        }
    }


    return 0;
}
