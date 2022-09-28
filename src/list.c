#include <stdlib.h>  
#include <stdio.h>
#include "list.h"
#include "utils.h"

List *newList() {
    List * myList = malloc(sizeof (List));
    myList->data = NULL;
    myList->nextList = NULL;
    myList->listSize = -1;
    return myList;
}

void L_push_back(List *list, int size, void *toAdd) {
    List *copyList = list;
        
    if (list->data == NULL) {
        list->data = toAdd;
        list->listSize = 0;
        list->nextList = NULL;
    } else {
        while (copyList->nextList != NULL) {
            copyList = copyList->nextList;
        }
        copyList->nextList = malloc(size + sizeof (List));
        copyList->nextList->data = toAdd;
        copyList->nextList->listSize = 0;
        copyList->nextList->nextList = NULL;
    }
    list->listSize++;
}

int L_getListSize(List * list) {
    return list->listSize;
}

List *L_getListElem(List *list, int pos) {
    List *copyList = list;
    int count = 0;
    while (count <= list->listSize) {
        if (count == pos)
            return copyList;
        count++;
        copyList = copyList->nextList;
    }

    return NULL;
}

void freeList(List *list) {
    for (List *temp=list; temp!=NULL; ) {
        List *tempNext=temp->nextList;
        tryFree(temp->data);
        tryFree(temp);
        temp=tempNext;
    }

    list=NULL;
}
