#pragma once

typedef struct LinkedList{
    void *data;
    struct LinkedList *nextList;
    int listSize;
} List;

List *newList();
void L_push_back(List *list, int size, void *toAdd);
int L_getListSize(List *list);
List *L_getListElem(List *list, int pos);