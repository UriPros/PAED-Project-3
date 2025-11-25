
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


int opt = 0;
int kopt = ' ';
int bopt = ' ';
int wopt = ' ';


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

char StartMenuKart(char kartOption) {
printf("\n");
printf("A. Circuit detection");
printf("B. Vehicle optimization\n");
printf("C. Go back\n");
printf("\n");
scanf("Wich tool do you want to use? %c", &kartOption);

return kartOption;
}
char StartMenuBrawl(char brawlOption) {
printf("\n");
printf("A. Add fighter");
printf("B. Remove fighther");
printf("C. Visual representation");
printf("D. Search fighter");
printf("E. Counterpick\n");
printf("F. Go back\n");
scanf("Wich tool do you want to use? %c", &brawlOption);

return brawlOption;
}

char StartMenuWonder(char wonderOption) {
printf("\n");
printf("A. Add element");
printf("B. Delete element");
printf("C. Lookup");
printf("D. Search by area");
printf("E. Statistics\n");
printf("F. Go back\n");
scanf("Wich tool do you want to use? %c", &wonderOption);

return wonderOption;
}

int main() {


int option = StartMenu(opt);

while (opt != 4) {
    switch(option) {

        case 1:
            char kartOption = StartMenuKart(kopt);
            switch(kartOption) {

                case 'A':
                    // funció
                    break;

                case 'B':
                    // funció
                    break;

                case 'C':
                    break;
            }

        case 2:
            char brawlOption = StartMenuBrawl(bopt);
            switch(brawlOption) {

                case 'A':
                    // funció
                    break;

                case 'B':
                    // funció
                    break;

                case 'C':
                    // funció
                    break;

                case 'D':
                    // funció
                    break;

                case 'E':
                    // funció
                    break;

                case 'F':
                    break;
            }
    
        case 3:
            char wonderOption = StartMenuWonder(wopt);
            switch(wonderOption) {

                case 'A':
                    // funció
                    break;

                case 'B':
                    // funció
                    break;

                case 'C':
                    // funció
                    break;

                case 'D':
                    // funció
                    break;

                case 'E':
                    // funció
                    break;

                case 'F':
                    break;
            }
            
        case 4:
            printf("Turning Super Structure Bros off\n");
            printf("._/\  ·  \___/\___/  ·  /\_.\n");
            break;
    }
}





return 0;
}
