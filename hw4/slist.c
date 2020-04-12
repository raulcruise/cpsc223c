#include "slist.h"
#include <stdlib.h>

snode* snode_create(int data, snode* next) {
  snode* temp = (snode*)malloc(sizeof(snode));
  temp->data = data;
  temp->next = next;
  return temp;
}
slist* slist_create(void) {
  slist* temp = (slist*)malloc(sizeof(slist));
  temp->head = NULL;
  temp->tail = NULL;
  temp->size = 0;
  return temp;
}

bool slist_empty(slist* list) {
  if (list->size == 0) {
    return true;
  }
  return false;
}
size_t slist_size(slist* list) {
  return list->size;
}

void slist_popfront(slist* list) {
  snode* temp = list->head;
  list->head = list->head->next;
  free(temp);
  --(list->size);
}
void slist_popback(slist* list) {
  snode* temp = list->head;
  while (temp->next != list->tail) {
    temp = temp->next;
  }
  free(temp->next);
  --(list->size);
  if(slist_empty(list)) {
    slist_clear(list);
  }
}

void slist_pushfront(slist* list, int data) {
  list->head = snode_create(data, list->head);
  ++(list->size);
  if (list->size == 1) {
    list->tail = list->head;
  }
}
void slist_pushback(slist* list, int data) {
  if (slist_empty(list)) {
    list->head = list->tail = snode_create(data, NULL);
    ++(list->size);
    return;
  }
  list->tail->next = snode_create(data, NULL);
  list->tail = list->tail->next;
  ++(list->size);
}

void slist_clear_(snode* node) {
  if (node->next != NULL) {
    slist_clear_(node->next);
  }
  free(node);
}

void slist_clear(slist* list) {
  slist_clear_(list->head);
  list->head = list->tail = NULL;
  list->size = 0;
}

void slist_print(slist* list, const char* msg) {
  printf("%s\n", msg);
  if (slist_empty(list)) {
    printf("Empty!\n");
    return;
  }
  snode* temp = list->head;
  while (temp != list->tail ) {
    printf("%d --> %p\n", temp->data, temp->next);
    temp = temp->next;
  }
  printf("%d --> %p\n", temp->data, temp->next);
}

int slist_front(slist* list) {
  return list->head->data;
}
int slist_back(slist* list) {
  return list->tail->data;
}

void slist_test(void);
