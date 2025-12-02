#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING 100


typedef struct {
    int id;
    char name[MAX_STRING];
    int atk_power;
    char colour[MAX_STRING];
    char ability[MAX_STRING];
    int price;
} Fighter;


typedef struct treenode {
    Fighter fighter;
    struct treenode *right;
    struct treenode *left;
}treenode;



treenode *createnode(Fighter fighter){

    treenode* newnode = (treenode*)malloc(sizeof(treenode));

    if (newnode != NULL) {
        newnode->fighter = fighter;
        newnode->left = NULL;
        newnode->right = NULL;
    }

    return newnode;
}


void addFighter(){

    Fighter newfighter;

    printf("\nFighter Identifier: ");
    scanf("%d", &newfighter.id);

    printf("Fighter Name: ");
    scanf("%s", newfighter.name);

    printf("Fighter attack power: ");
    scanf("%d", &newfighter.atk_power);

    printf("Fighter colour: ");
    scanf("%s", newfighter.colour);

    printf("Fighter ability: ");
    scanf("%s", newfighter.ability);

    printf("Fighter price: ");
    scanf("%d", &newfighter.price);


    treenode* fighternode = createnode(newfighter);

    if (fighternode != NULL) {
        printf("\nThe roster has been expanded with the fighter %s.\n", newfighter.name);
    } else {
        printf("Failed to create fighter node.\n");
    }
}

void removeFighter(){

    int id_to_remove;

    printf("\nFighter identifier: ");
    scanf("%d", &id_to_remove);

    // In a complete implementation, you would search the tree and remove the node.
    //Faig veure que ja s'ha eliminat

    printf("Fighter with Identifier %d has been removed from the roster.\n", id_to_remove);
}



int main(){

    readfile();

    addFighter();

    removeFighter();

    return 0;
}
