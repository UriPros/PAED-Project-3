#ifndef TREES_H
#define TREES_H


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

treenode* root;


int max(int a, int b);
int getHeight(treenode* node);
int getBalanceFactor(treenode* node);
treenode *rightRotate(treenode *node);
treenode *leftRotate(treenode *node);
treenode *createnode(Fighter fighter);
void insertNode(treenode* *root, Fighter fighter);
int readTreeFile();
void addFighter(int numFighters);
treenode *findMin (treenode *node);
void deleteNode(treenode* *root, int power_lvl);
treenode* findFighterById(treenode* node, int id);
void removeFighter();
void printTabs(int level);
void printColoredText(Fighter fighter);
void printNode(treenode* node, char isLeft);
void printTree(treenode* node, int level, char isLeft);
void visualRepresentation(treenode* root);
int findNode(treenode* root, int power_lvl, Fighter *foundFighter);
void searchFighter(treenode* root);
int findCounters(treenode* node, int min_atk, int max_atk);
void counterPick();


#endif
