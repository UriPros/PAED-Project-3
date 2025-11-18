#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int option = 0;

int StartMenu(int option) {
    printf("\n._/\\ Super Structure Bros /\\_.\n");
    printf("\n");
    printf("1. Kart (Graphs)\n");
    printf("2. Brawl (Trees)\n");
    printf("3. Wonder (Tables)\n");
    printf("\n");
    printf("4. Stop\n");
    printf("\n");
    scanf("Pick a game: %d", &option);

    return option;
}



int main() {

    
    option = StartMenu(option);


    return 0;
}
