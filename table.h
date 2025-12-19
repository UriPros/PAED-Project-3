#ifndef TABLES_H
#define TABLES_H

#define MAX 100

typedef struct {
    char name[MAX];
    char world[MAX];
    float latitude;
    float longitude;
} Element;

typedef struct ElementNode {
    Element data;
    struct ElementNode* next;
} ElementNode;

ElementNode* table[MAX];

int hash(char name[]);
void readTableFile();
void add_element();
void remove_element();
void lookup_element();
void search_by_area();
void statistics();

#endif
