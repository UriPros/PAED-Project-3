#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

ElementNode* list_add(ElementNode* head, Element e) {
    ElementNode* newNode = malloc(sizeof(ElementNode));
    newNode->data = e;
    newNode->next = head;
    return newNode;
}

Element* list_get(ElementNode* head, char* name) {
    while (head != NULL) {
        if (strcmp(head->data.name, name) == 0) {
            return &head->data;
        }
        head = head->next;
    }
    return NULL;
}

ElementNode* list_remove(ElementNode* head, char* name) {
    ElementNode* curr = head;
    ElementNode* prev = NULL;

    while (curr != NULL) {
        if (strcmp(curr->data.name, name) == 0) {
            if (prev == NULL)
                head = curr->next;
            else
                prev->next = curr->next;

            free(curr);
            return head;
        }
        prev = curr;
        curr = curr->next;
    }

    return head;
}

void list_free(ElementNode* head) {
    while (head != NULL) {
        ElementNode* temp = head;
        head = head->next;
        free(temp);
    }
}
