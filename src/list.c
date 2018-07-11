#include <stdio.h>
#include <stdlib.h>
#include "list.h"

List *make_list()
{
    List *list = malloc(sizeof(List));
    list->len = 0;
    list->head = list->tail = NULL; 
    return list; 
}

Listnode *make_node(void *n)
{
    Listnode *node = malloc(sizeof(Listnode));
    node->elem = n;
    node->next = NULL;
    return node;
}

void list_append(List *list, void *elem) {
    Listnode *node = make_node(elem);
    if (!list->head) list->head = node; 
    else list->tail->next = node;
    list->tail = node;
    list->len++;
}

int list_len(List *list) {
    return list->len;
}

Iter *list_iter(List *list) {
    Iter *r = malloc(sizeof(Iter));
    r->ptr = list->head;
    return r;
}

void *iter_next(Iter *iter) {
    if (!iter->ptr)
        return NULL;
    void *r = iter->ptr->elem;
    iter->ptr = iter->ptr->next;
    return r;
}

bool iter_end(Iter *iter) {
    return !iter->ptr;
}
