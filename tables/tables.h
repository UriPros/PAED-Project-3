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

ElementNode* list_add(ElementNode* head, Element e);
void add_element(ElementNode* table[]);

ElementNode* list_remove(ElementNode* head, char* name);
void remove_element(ElementNode* table[]);

Element* list_get(ElementNode* head, char* name);
void lookup_element(ElementNode* table[]);

void search_by_area(ElementNode* table[]);


#endif
