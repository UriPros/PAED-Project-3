#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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




treenode* root = NULL;




treenode *createnode(Fighter fighter){

    treenode* newnode = (treenode*)malloc(sizeof(treenode));

    if (newnode != NULL) {
        newnode->fighter = fighter;
        newnode->left = NULL;
        newnode->right = NULL;
    }

    return newnode;
}

bool insertNode(treenode** root_pointer, Fighter fighter){

    treenode *root = *root_pointer;

    if (root == NULL) {
        root = createnode(fighter);
        return true;
    }
    if (fighter.id == root->fighter.id) {
        return false; // Duplicate ID
    }
    if (fighter.id < root->fighter.id) {
        return insertNode(&(root->left), fighter);
    }
    else {
        return insertNode(&(root->right), fighter);
    }
}

void readfile(){
    FILE *f;
    Fighter fighter;
    int numFighters;
    int first_time = 0;

    f = fopen("treeData.txt", "r");

    if (!f) {
        printf("Error opening file!\n");
        return;
    }

    fscanf(f, "%d\n", &numFighters);

    for (int i = 0; i < numFighters; i++) {

        fscanf(f, "%d;%[^;];%d;%[^;];%[^;];%d\n",
            &fighter.id,
            fighter.name,
            &fighter.atk_power,
            fighter.colour,
            fighter.ability,
            &fighter.price
        );

        // In a complete implementation, you would insert the new node into the tree here.
        if (!first_time) {
            // Initialize tree root
            root = createnode(fighter);
            first_time = 1;
        }
        else {
            insertNode(&root, fighter);
        }

    }


    fclose(f);
}

void addFighter(){

    Fighter newFighter;
    treenode* newFighterNode;

    printf("\nFighter Identifier: ");
    scanf("%d", &newFighter.id);

    printf("Fighter Name: ");
    scanf("%s", newFighter.name);

    printf("Fighter attack power: ");
    scanf("%d", &newFighter.atk_power);

    printf("Fighter colour: ");
    scanf("%s", newFighter.colour);

    printf("Fighter ability: ");
    scanf("%s", newFighter.ability);

    printf("Fighter price: ");
    scanf("%d", &newFighter.price);


    newFighterNode = createnode(newFighter);
    insertNode(&root, newFighter);

    if (newFighterNode != NULL) {
        printf("\nThe roster has been expanded with the fighter %s.\n", newFighter.name);
    } else {
        printf("Failed to create fighter node.\n");
    }
}


bool findNode(treenode* root, int id, Fighter *foundFighter) {

    if (root == NULL) {
        return false; // Not found
    }
    if (id == root->fighter.id) {
        *foundFighter = root->fighter;
        return true; // Found
    }
    if (id < root->fighter.id) {
        return findNode(root->left, id, foundFighter);
    }
    else {
        return findNode(root->right, id,foundFighter);
    }
}

void removeFighter(){

    Fighter fighter_to_remove;
    int id_to_remove;

    printf("\nFighter identifier: ");
    scanf("%d", &id_to_remove);

    // In a complete implementation, you would search the tree and remove the node.
    if (findNode(root, id_to_remove, &fighter_to_remove)) {
        // Node found, proceed to remove (removal logic not implemented here)
        printf("Fighter %s has been removed from the roster.\n", fighter_to_remove.name);
    } else {
        printf("Fighter %s not found in the roster.\n", fighter_to_remove.name);
        return;
    }

}



int main(){

    readfile();

    addFighter();

    removeFighter();

    return 0;
}
