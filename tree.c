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

void insertNode(treenode** root_pointer, Fighter fighter){

    treenode *node = *root_pointer;

    if (node == NULL) {
        (*root_pointer) = createnode(fighter);
        return;
    }
    if (fighter.id == node->fighter.id) {
        //Nothing // Duplicate ID
    }
    if (fighter.id < node->fighter.id) {
        return insertNode(&(node->left), fighter);
    }
    else {
        return insertNode(&(node->right), fighter);
    }
}

int readfile(){
    FILE *f;
    Fighter fighter;
    int numFighters;

    f = fopen("treeData.txt", "r");

    if (!f) {
        printf("Error opening file!\n");
        return 0;
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

        insertNode(&root, fighter);

    }

    fclose(f);

    return numFighters;
}

void addFighter(int numFighters){

    Fighter newFighter;

    printf("\nFighter Identifier: ");
    scanf("%d", &newFighter.id);
    while (newFighter.id <= numFighters) {
        printf("Invalid ID. Please enter a positive integer: ");
        scanf("%d", &newFighter.id);
    }

    printf("Fighter Name: ");
    scanf(" %99[^\n]", newFighter.name);

    printf("Fighter attack power: ");
    scanf("%d", &newFighter.atk_power);

    printf("Fighter colour: ");
    scanf("%s", newFighter.colour);

    printf("Fighter ability: ");
    scanf("%s", newFighter.ability);

    printf("Fighter price: ");
    scanf("%d", &newFighter.price);

    insertNode(&root, newFighter);

    printf("\nThe roster has been expanded with the fighter %s.\n", newFighter.name);
}


int findNode(treenode* root, int id, Fighter *foundFighter) {

    if (root == NULL) {
        return 0; // Not found
    }
    if (id == root->fighter.id) {
        *foundFighter = root->fighter;
        return 1; // Found
    }
    if (id < root->fighter.id) {
        return findNode(root->left, id, foundFighter);
    }
    else {
        return findNode(root->right, id,foundFighter);
    }
}

treenode *findMin (treenode *node) {
    treenode *current = node;

    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}

void deleteNode(treenode** root_pointer, int id) {

    treenode *node = *root_pointer;
    treenode *inOrderSuccessor;

    if (node == NULL) {
        return;
    }

    if (id < node->fighter.id) {
        return deleteNode(&(node->left), id);
    }
    else if (id > node->fighter.id){
        return deleteNode(&(node->right), id);
    }
    else {
        // Node found, perform deletion (not implemented)
        if (node->left == NULL) {
            *root_pointer = node->right;
            free(node);
        }
        else if (node->right == NULL) {
            *root_pointer = node->left;
            free(node);
        }
        else {
            inOrderSuccessor = findMin(node->right);
            node->fighter = inOrderSuccessor->fighter;
            deleteNode(&(node->right), inOrderSuccessor->fighter.id);
        }
    }
}


void removeFighter(){

    Fighter fighter_to_remove;
    int id_to_remove;
    int found = 0;

    printf("\nFighter identifier: ");
    scanf("%d", &id_to_remove);

    found = findNode(root, id_to_remove, &fighter_to_remove);

    // In a complete implementation, you would search the tree and remove the node.
    if (found) {
        deleteNode(&root, id_to_remove);
        printf("Fighter %s has been removed from the roster.\n", fighter_to_remove.name);
    } else {
        printf("Fighter with ID %d not found in the roster.\n", id_to_remove);
        return;
    }
}


void printtabs(int level) {
    for (int i = 0; i < level; i++) {
        printf("\t");
    }
}

void printtree(treenode* node, int level) {
    if (node == NULL) {
        printtabs(level);
        printf("NULL\n");
        return;
    }

    printtabs(level);
    printf("id = %d\n", node->fighter.id);
    
    printtabs(level);
    printf("right\n");
    printtree(node->right, level + 1);

    printtabs(level);
    printf("left\n");
    printtree(node->left, level + 1);

    printtabs(level);
    printf("done\n");
    
}

void visualRepresentation(treenode* root){
    printf("\nVisual representation of the fighter roster tree:\n\n");
    printtree(root, 0);
}


int main(){

    int numFighters = readfile();

    addFighter(numFighters);

    removeFighter();

    visualRepresentation(root);

    return 0;
}
