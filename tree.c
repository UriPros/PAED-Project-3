#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING 100


typedef struct {
    int id;
    char name[MAX_STRING];
    int power_lvl;
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
    if (fighter.power_lvl == node->fighter.power_lvl) {
        //Nothing // Duplicate ID
    }
    if (fighter.power_lvl < node->fighter.power_lvl) {
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
            &fighter.power_lvl,
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
    scanf("%d", &newFighter.power_lvl);

    printf("Fighter colour: ");
    scanf("%s", newFighter.colour);

    printf("Fighter ability: ");
    scanf("%s", newFighter.ability);

    printf("Fighter price: ");
    scanf("%d", &newFighter.price);

    insertNode(&root, newFighter);

    printf("\nThe roster has been expanded with the fighter %s.\n", newFighter.name);
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

    // In a complete implementation, you would search the tree and remove the node.
    deleteNode(&root, id_to_remove);
    printf("Fighter %s has been removed from the roster.\n", fighter_to_remove.name);
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
//falta implementar els colors i com volem que sigui el tree visualment
void visualRepresentation(treenode* root){
    printf("\nVisual representation of the fighter roster tree:\n\n");
    printtree(root, 0);
}



int findNode(treenode* root, int power_lvl, Fighter *foundFighter) {

    if (root == NULL) {
        return 0; // Not found
    }
    if (power_lvl == root->fighter.power_lvl) {
        *foundFighter = root->fighter;
        return 1; // Found
    }
    if (power_lvl < root->fighter.power_lvl) {
        return findNode(root->left, power_lvl, foundFighter);
    }
    else {
        return findNode(root->right, power_lvl,foundFighter);
    }
}

void searchFighter(treenode* root){

    int power_lvl;
    Fighter foundFighter;
    int found = 0;

    printf("Power level to search for: ");
    scanf("%d", &power_lvl);

    //et fa search per ID -> s'ha de canviar a power level
    found = findNode(root, power_lvl, &foundFighter);

    if (found) {
        printf("\nFighter found:\n");
        printf("ID: %d\n", foundFighter.id);
        printf("Name: %s\n", foundFighter.name);
        printf("Attack Power: %d\n", foundFighter.power_lvl);
        printf("Colour: %s\n", foundFighter.colour);
        printf("Ability: %s\n", foundFighter.ability);
        printf("Price: %d\n", foundFighter.price);
    } else {
        printf("Fighter with power level: %d not found in the roster.\n", power_lvl);
    }
}



int countCounters(treenode* node, int min_atk, int max_atk){

    int count = 0;

    if (node == NULL) {
        return 0;
    }

    if (node->fighter.power_lvl >= min_atk && node->fighter.power_lvl <= max_atk) {
        count++;
    }
    
    count = count + countCounters(node->left, min_atk, max_atk);
    count = count + countCounters(node->right, min_atk, max_atk);

    return count;
}

//Inorder traversal to find and print counters (first left, then root, then right)
void findCounters(treenode* node, int min_atk, int max_atk){

    if (node == NULL) {
        return;
    }

    if (node->fighter.power_lvl <= max_atk) {
        findCounters(node->right, min_atk, max_atk);
    }
    if (node->fighter.power_lvl >= min_atk && node->fighter.power_lvl <= max_atk) {
        printf("\t*%s (%d): %d\n",node->fighter.name, node->fighter.id, node->fighter.power_lvl);
    }
    if (node->fighter.power_lvl >= min_atk) {
        findCounters(node->left, min_atk, max_atk);
    }
}

void counterPick(){
       
    int min_atk, max_atk;
    int num_counters = 0;

    printf("\nMinimum attak power level: ");
    scanf("%d", &min_atk);
    printf("Maximum attak power level: ");
    scanf("%d", &max_atk);

    num_counters = countCounters(root, min_atk, max_atk);
    printf("\n %d fighters found with attack power level between %d and %d.\n\n", num_counters, min_atk, max_atk);

    findCounters(root, min_atk, max_atk);
}


int main(){

    int numFighters = readfile();

    addFighter(numFighters);

    removeFighter();

    visualRepresentation(root);

    searchFighter(root);

    counterPick();

    return 0;
}
