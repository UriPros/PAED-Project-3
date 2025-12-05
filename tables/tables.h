#ifndef TABLES_H
#define TABLES_H

#define MAX 100

#include "linkedlist.h"

typedef struct Element {
    char name[MAX];
    char world[MAX];
    float latitude;
    float longitude;
} Element;

int hash(char* name);

ElementNode* table_add(ElementNode* head, Element e);
void add_element(ElementNode* table[]);

ElementNode* table_remove(ElementNode* head, char* name);
void remove_element(ElementNode* table[]);


#endif