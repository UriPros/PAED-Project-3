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
    int height;
}treenode;


treenode* root = NULL;

int max(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

int getHeight(treenode* node) {

    int leftHeight, rightHeight;

    if (node == NULL) {
        return 0;
    }

    leftHeight = getHeight(node->left);
    rightHeight = getHeight(node->right);

    if (leftHeight > rightHeight) {
        return leftHeight + 1;
    }
    else {
        return rightHeight + 1;
    }
}

int getBalanceFactor(treenode* node) {
    if (node == NULL) {
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}

treenode *rightRotate(treenode *node) {
    treenode *prev = node->left;
    treenode *middle = prev->right;

    // Perform rotation
    prev->right = node;
    node->left = middle;

    // Update heights
    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;  //now this node is below the prev
    prev->height = max(getHeight(prev->left), getHeight(prev->right)) + 1;  //now this node is the new root

    // Return old root to check against balancing
    return prev; 
}

treenode *leftRotate(treenode *node) {
    treenode *prev = node->right;
    treenode *middle = prev->left;

    // Perform rotation
    prev->left = node;
    node->right = middle;

    // Update heights
    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;  //now this node is below the prev
    prev->height = max(getHeight(prev->left), getHeight(prev->right)) + 1;  //now this node is the new root

    // Return old root to check against balancing
    return prev; 
}


treenode *createnode(Fighter fighter){
    treenode* newnode = (treenode*)malloc(sizeof(treenode));
    if (newnode != NULL) {
        newnode->fighter = fighter;
        newnode->left = NULL;
        newnode->right = NULL;
        newnode->height = 1;
    }
    return newnode;
}

void insertNode(treenode* *root, Fighter fighter){

    treenode *node = *root;

    if (node == NULL) {
        *root = createnode(fighter);
        return;
    }
    if (fighter.power_lvl < node->fighter.power_lvl) {
        insertNode(&node->left, fighter);
    }
    else if (fighter.power_lvl > node->fighter.power_lvl) {
        insertNode(&node->right, fighter);
    }
    else {
        //duplicate power levels: do nothing
        return;
    }

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    int balance_factor = getBalanceFactor(node);

    // Left Left Case
    if (balance_factor > 1 && fighter.power_lvl < node->left->fighter.power_lvl) {
        *root = rightRotate(node);
        return;
    }
    // Right Right Case
    if (balance_factor < -1 && fighter.power_lvl > node->right->fighter.power_lvl) {
        *root = leftRotate(node);
        return;
    }
    // Left Right Case
    if (balance_factor > 1 && fighter.power_lvl > node->left->fighter.power_lvl) {
        node->left = leftRotate(node->left);
        *root = rightRotate(node);
        return;
    }
    // Right Left Case
    if (balance_factor < -1 && fighter.power_lvl < node->right->fighter.power_lvl) {
        node->right = rightRotate(node->right);
        *root = leftRotate(node);
        return;
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

    while (node && node->left != NULL) {
        node = node->left;
    }
    return node;
}

void deleteNode(treenode* *root, int power_lvl) {

    treenode *node = *root;
    treenode *inOrderSuccessor;
    treenode *temp;

    if (node == NULL) {
        return;
    }

    if (power_lvl < node->fighter.power_lvl) {
        deleteNode(&node->left, power_lvl);
    }
    else if (power_lvl > node->fighter.power_lvl){
        deleteNode(&node->right, power_lvl);
    }
    else {
        // Node found, perform deletion (not implemented)
        // node with only one child or no child
        if (node->left == NULL) {
            *root = node->right;
            free(node);
            return;
        }
        else if (node->right == NULL) {
            *root = node->left;
            free(node);
            return;
        }
        else {
            inOrderSuccessor = findMin(node->right);
            node->fighter = inOrderSuccessor->fighter;
            deleteNode(&node->right, inOrderSuccessor->fighter.power_lvl);
        }
    }

    if (*root == NULL) {
        return;
    }

    node = *root;

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    int balance_factor = getBalanceFactor(node);

    // Left Left Case
    if (balance_factor > 1 && getBalanceFactor(node->left) >= 0) {
        *root = rightRotate(node);
        return;
    }
    // Right Right Case
    if (balance_factor < -1 && getBalanceFactor(node->right) <= 0) {
        *root = leftRotate(node);
        return;
    }
    // Left Right Case
    if (balance_factor > 1 && getBalanceFactor(node->left) < 0) {
        node->left = leftRotate(node->left);
        *root = rightRotate(node);
        return;
    }
    // Right Left Case
    if (balance_factor < -1 && getBalanceFactor(node->right) > 0) {
        node->right = rightRotate(node->right);
        *root = leftRotate(node);
        return;
    }

}


treenode* findFighterById(treenode* node, int id) {

    treenode* foundNode;

    if (node == NULL) {
        return NULL;
    }
    if (id == node->fighter.id) {
        return node;
    }

    foundNode = findFighterById(node->left, id);
    if (foundNode != NULL) {
        return foundNode;
    }

    return findFighterById(node->right, id);
}


void removeFighter(){

    treenode* node_to_remove;
    int id_to_remove;
    int found = 0;

    printf("\nFighter identifier: ");
    scanf("%d", &id_to_remove);

    node_to_remove = findFighterById(root, id_to_remove);

    if (node_to_remove == NULL) {
        printf("Fighter with ID %d not found.\n", id_to_remove);
        return;
    }

    // In a complete implementation, you would search the tree and remove the node.
    deleteNode(&root, node_to_remove->fighter.power_lvl);
    printf("Fighter %s has been removed from the roster.\n", node_to_remove->fighter.name);
}


//falta implementar els colors i com volem que sigui el tree visualment
void printTabs(int level) {
    for (int i = 0; i < level; i++) {
        printf("\t");
    }
}

void printNode(treenode* node, char isLeft) {
    if (isLeft)
        printf("┌── ");
    else
        printf("└── ");

    printf("%s (%d)\n", node->fighter.name, node->fighter.power_lvl);
}

//tree traversal (right -> node -> left)
void printTree(treenode* node, int level, char isLeft) {
    if (node == NULL) {
        return;
    }

    // Print right  first (on top)
    printTree(node->right, level + 1, 1);

    // Print current node
    printTabs(level);
    printNode(node, isLeft);

    // Print left  (below)
    printTree(node->left, level + 1, 0);
}

void visualRepresentation(treenode* root) {
    printf("\nVisual representation of the fighter roster tree:\n");
    printTree(root, 0, 0);
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


//Inorder traversal to find and print counters (first left, then root, then right)
int findCounters(treenode* node, int min_atk, int max_atk) {

    int count = 0;

    if (node == NULL) {
        return 0;
    }

    if (node->fighter.power_lvl <= max_atk) {
        count += findCounters(node->right, min_atk, max_atk);
    }
    if (node->fighter.power_lvl >= min_atk && node->fighter.power_lvl <= max_atk) {
        printf("\t*%s (%d): %d\n",node->fighter.name, node->fighter.id, node->fighter.power_lvl);
        count++;
    }
    if (node->fighter.power_lvl >= min_atk) {
        count += findCounters(node->left, min_atk, max_atk);
    }

    return count;
}

void counterPick(){
       
    int min_atk, max_atk;
    int num_counters = 0;

    printf("\nMinimum attak power level: ");
    scanf("%d", &min_atk);
    printf("Maximum attak power level: ");
    scanf("%d", &max_atk);


    printf("\nWe are looking for fighters with attack power level between %d and %d.\n\n", min_atk, max_atk);
    printf("These are the fighters that can counter your opponent:\n\n");
    
    num_counters = findCounters(root, min_atk, max_atk);

    printf("\nTotal number of counters found: %d\n\n", num_counters);
    
}


int main(){

    int numFighters = readfile();

    //addFighter(numFighters);
    
    //removeFighter();

    visualRepresentation(root);

    //searchFighter(root);

    //counterPick();

    return 0;
}
