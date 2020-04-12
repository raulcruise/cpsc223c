#include "dlist.h"
#include <stdlib.h>

dnode* dnode_create(int data, dnode* prev, dnode* next) {
  dnode* temp = (dnode*)malloc(sizeof(dnode));
  temp->data = data;
  temp->prev = prev;
  temp->next = next;
  return temp;
}
dlist* dlist_create(void) {
  dlist* ptr = (dlist*)malloc(sizeof(dlist));
  return ptr;
}

bool dlist_empty(dlist* list) {
  if (list->size == 0) {
    return true;
  }
  return false;
}
size_t dlist_size(dlist* list) {
  return list->size;
}

void dlist_popfront(dlist* list) {
  dnode* temp = list->head;
  list->head = list->head->next;
  free(temp);
  --(list->size);
  if(dlist_empty(list)) {
    dlist_clear(list);
  }
}
void dlist_popback(dlist* list) {
  dnode* temp = list->tail;
  list->tail = list->tail->prev;
  free(temp);
  --(list->size);
  if(dlist_empty(list)) {
    dlist_clear(list);
  }
}

void dlist_pushfront(dlist* list, int data) {
  if (list->size == 0) {
    list->tail = list->head = dnode_create(data, NULL, NULL);
  }
  list->head = dnode_create(data, NULL, list->head);
  list->head->next->prev = list->head;
  ++(list->size);
}
void dlist_pushback(dlist* list, int data) {
  if (list->size == 0) {
    list->head = list->tail = dnode_create(data, NULL, NULL);
    ++(list->size);
    return;
  }
  list->tail->next = dnode_create(data, list->tail, NULL);
  list->tail = list->tail->next;
  ++(list->size);
}

void dlist_clear(dlist* list) {
  dnode* temp = list->head->next;
  do {
    free(temp->prev);
    temp = temp->next;
  } while (temp != list->tail);
  free(temp);

  list->head = list->head = NULL;
  list->size = 0;
}
void dlist_print(dlist* list, const char* msg) {
  printf("%s\n", msg);
  if (dlist_empty(list)) {
    printf("Empty!\n");
    return;
  }
  dnode* temp = list->head;
  while (temp != list->tail ) {
    printf("%11p <-- %d (%p) --> %p\n",temp->prev, temp->data, temp, temp->next);
    temp = temp->next;
  }
  printf("%11p <-- %d (%p) --> %p\n",temp->prev, temp->data, temp, temp->next);

}

int dlist_front(dlist* list) {
  return list->head->data;
}
int dlist_back(dlist* list) {
  return list->tail->data;
}

void dlist_test(void);
