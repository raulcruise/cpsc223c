//
//  tree.c
//  tree
//
//  Created by William McCarthy on 134//20.
//  Copyright © 2020 William McCarthy. All rights reserved.
//

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "tree.h"
#include "utils.h"

#define MAXCOUNT 120

typedef struct llnode llnode;
struct llnode {
  const char* word;
  llnode* next;
};

llnode* llnode_create(const char* word) {
  llnode* p = (llnode*)malloc(sizeof(llnode));
  p->word = strdup(word);    // copy of word allocated on heap
  p->next = NULL;
  return p;
}

llnode* globallist[MAXCOUNT];

//-------------------------------------------------

tnode* tnode_create(const char* word, int line) {
  tnode* p = (tnode*)malloc(sizeof(tnode));
  p->word = strdup(word);    // copy of word allocated on heap
  p->count = 1;
  p->left = NULL;
  p->right = NULL;
  p->lines[0] = line;
  p->numlines = 1;

  return p;
}

void tnode_delete(tnode* p) {
    free((void*)p->word);
    free(p);
}

tree* tree_create(void) {
  tree* p = (tree*)malloc(sizeof(tree));
  p->root = NULL;
  p->size = 0;

  return p;
}

static void tree_deletenodes(tree* t, tnode* p) {
  if (p == NULL) { return; }

  tree_deletenodes(t, p->left);
  tree_deletenodes(t, p->right);
  tnode_delete(p);
  t->size--;
}

void tree_clear(tree* t) {
  tree_delete(t);
  t->root = NULL;
  t->size = 0;
}

void tree_delete(tree* t) { tree_deletenodes(t, t->root); }

bool tree_empty(tree* t) { return t->size == 0; }
size_t tree_size(tree* t) { return t->size; }

static tnode* tree_addnode(tree* t, tnode** p, const char* word, int line) {
  int compare;

  if (*p == NULL) {
    *p = tnode_create(word, line);
  } else if ((compare = strcmp(word, (*p)->word)) == 0) {
    (*p)->count++;
    if ((*p)->lines[(*p)->numlines - 1] != line) {
      (*p)->lines[(*p)->numlines++] = line;
    }
  } else if (compare < 0) { tree_addnode(t, &(*p)->left, word, line);
  } else {
    tree_addnode(t, &(*p)->right, word, line);
  }

  return *p;
}

tnode* tree_add(tree* t, const char* word, int line) {
  tnode* p = tree_addnode(t, &(t->root), word, line);
  t->size++;

  return p;
}

static void tree_printme(tree* t, tnode* p) {
  if (p->count > 1) { printf(" %d --", p->count); }
  printf("\t%s", p->word);
  printf("\t\t-- [%d", p->lines[0]);
  for (int i = 1; i < p->numlines; ++i) {
    printf(", %d", p->lines[i]);
  }
  printf("]");
  printf("\n");
}

static void tree_addbycountme(tree* t, tnode* p) {
  llnode* temp;
  if ((temp = globallist[p->count]) == NULL) {
    globallist[p->count] = llnode_create(p->word);
  } else {
    while (temp->next != NULL) {
      temp = temp->next;
    }
    temp->next = llnode_create(p->word);
  }
}

static void tree_addbycountnodes(tree* t, tnode* p) {
  if (p == NULL) { return; }

  tree_addbycountnodes(t, p->left);
  tree_addbycountme(t, p);
  tree_addbycountnodes(t, p->right);
}

void tree_addbycount(tree* t) {
  tree_addbycountnodes(t, t->root);
}


static void tree_printnodes(tree* t, tnode* p) {
  if (p == NULL) { return; }

  tree_printnodes(t, p->left);
  tree_printme(t, p);
  tree_printnodes(t, p->right);
}

static void tree_printnodes_preorder(tree* t, tnode* p) {
  if (p == NULL) { return; }

  tree_printme(t, p);
  tree_printnodes_preorder(t, p->left);
  tree_printnodes_preorder(t, p->right);
}

static void tree_printnodes_postorder(tree* t, tnode* p) {
  if (p == NULL) { return; }

  tree_printnodes_postorder(t, p->left);
  tree_printnodes_postorder(t, p->right);
  tree_printme(t, p);
}

void tree_print(tree* t) {    // INORDER-printing
  tree_printnodes(t, t->root);
  printf("\n");
}

void tree_print_preorder(tree* t) {
  tree_printnodes_preorder(t, t->root);
}

void tree_print_postorder(tree* t) {
  tree_printnodes_postorder(t, t->root);
}

void tree_printbycount() {
  llnode* p;
  for (int i = MAXCOUNT - 1; i >= 0; --i) {
    if ((p = globallist[i]) != NULL) {
      while (p != NULL) {
        printf("%d --\t%s\n", i, p->word);
        p = p->next;
      }
    }
  }
}
//void tree_print_levelorder(tree* t);

int is_delimiter(int c) {
  switch(c) {
    case ',':
    case '?':
    case ';':
    case '(':
    case ')':
    case '!':
      return 1;
      break;
    default:
    return 0;
  }
}

void tree_function() {
  tree* t = tree_create();
  char word[60];
  char c;
  int line = 1;
  do {
    int i = 0;
    while ((c = getc(stdin)) != ' ' && c != EOF && c != '\n' && !is_delimiter(c)) {
      word[i++] = tolower(c);
    }
    if (i > 0) {
      word[i] = '\0';
      tree_add(t, word, line);
    }
    if (c == '\n') { ++line; }
  } while (c != EOF);

  tree_addbycount(t);
  tree_printbycount();
  /* These work too
  tree_print_preorder(t);
  tree_print_postorder(t);
  */
  tree_clear(t);
}


int main(int argc, const char* argv[]) {
  tree_function();

  return 0;
}
