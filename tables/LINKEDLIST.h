#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "tables.h"

typedef struct ElementNode {
    Element data;
    struct ElementNode* next;
} ElementNode;


ElementNode* list_add(ElementNode* head, Element e);
Element* list_get(ElementNode* head, char* name);
ElementNode* list_remove(ElementNode* head, char* name);
void list_free(ElementNode* head);

#endif
