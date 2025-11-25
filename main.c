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

char StartMenuKart(char) {
    printf("\n");
    printf("A. Circuit detection");
    printf("B. Vehicle optimization\n");
    printf("C. Go back\n");
    printf("\n");
    scanf("Wich tool do you want to use? %c", &);

    return ;
}
char StartMenuBrawl(char) {
    printf("\n");
    printf("A. Add fighter");
    printf("B. Remove fighther");
    printf("C. Visual representation");
    printf("D. Search fighter");
    printf("E. Counterpick\n");
    printf("F. Go back\n");
    scanf("Wich tool do you want to use? %c", &);

    return ;
}

char StartMenuWonder(char) {
    printf("\n");
    printf("A. Add element");
    printf("B. Delete element");
    printf("C. Lookup");
    printf("D. Search by area");
    printf("E. Statistics\n");
    printf("F. Go back\n");
    scanf("Wich tool do you want to use? %c", &);

    return ;
}

int main() {

    
    option = StartMenu(option);


    return 0;
}
