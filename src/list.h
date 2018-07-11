#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

typedef struct Listnode{
    void *elem;
    struct Listnode *next; 
}Listnode;

typedef struct List{
    int len;
    Listnode *head; 
    Listnode *tail; 
}List;

typedef struct Iter{
    Listnode *ptr;
}Iter;

List *make_list();
Listnode *make_node(void *n);
void list_append(List *list, void *elem);
int list_len(List *list);
Iter *list_iter(List *list);
void *iter_next(Iter *iter);
bool iter_end(Iter *iter);


#define EMPTY_LIST                                          \
    (&(LIST){ .len = 0; .head = NULL; .tail = NULL}) 

#endif
