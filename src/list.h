#pragma once

typedef struct LinkedList{
    void *data;
    struct LinkedList *nextList;
    int listSize;
} List;

List *newList();
List *L_getListElem(List *list, int pos);
int L_getListSize(List *list);
void L_push_back(List *list, int size, void *toAdd);
void freeList(List *list);
