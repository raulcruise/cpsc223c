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

int N_CHARS = 6;

//-------------------------------------------------
int isnotnoise(char* word) {
  if (strcmp(word, "the") == 0) return 0;
  if (strcmp(word, "of") == 0) return 0;
  if (strcmp(word, "and") == 0) return 0;
  if (strcmp(word, "a") == 0) return 0;
  if (strcmp(word, "to") == 0) return 0;
  if (strcmp(word, "in") == 0) return 0;
  if (strcmp(word, "is") == 0) return 0;
  if (strcmp(word, "that") == 0) return 0;
  if (strcmp(word, "or") == 0) return 0;
  if (strcmp(word, "it") == 0) return 0;
  return 1;
}

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

static void tree_printnodes_n(tree* t, tnode* p) {
  static char prev[100];
  static bool firsttime = true;
  if (firsttime) {
    memset(prev, 0, sizeof(prev));
    strcpy(prev, p->word);
  }

  int compare = strncmp(prev, p->word, N_CHARS);
  if (compare != 0) {
    printf("\n");
  }

  strcpy(prev, p->word);
  printf("%s ", p->word);

  firsttime = false;
}

static void tree_printnodes(tree* t, tnode* p) {
  if (p == NULL) { return; }

  tree_printnodes(t, p->left);
  tree_printnodes_n(t, p);
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

int isDataType(const char* word) {
  if (strcmp(word, "int") == 0) return 1;
  if (strcmp(word, "char*") == 0) return 1;
  if (strcmp(word, "char") == 0) return 1;
  if (strcmp(word, "unsigned") == 0) return 1;
  if (strcmp(word, "long") == 0) return 1;
  if (strcmp(word, "double") == 0) return 1;
  if (strcmp(word, "float") == 0) return 1;
  if (strcmp(word, "long") == 0) return 1;
  return 0;
}
//void tree_print_levelorder(tree* t);

void tree_function(int argc, const char* argv[]) {
  if (argc == 3) {
    N_CHARS = atoi(argv[2]);
  }
  char delimiters[] = " .,;[](){=><\"\\/+%!-&|}*\n";
  tree* t = tree_create();
  char line[1000];
  while(fgets(line, sizeof(line), stdin)) {
    char* word = strtok(line, delimiters);
      while ((word = strtok(NULL, delimiters)) != NULL) {
        tree_add(t, word, 0);
    }
  }

  tree_print(t);
  /* These work too
  tree_print_preorder(t);
  tree_print_postorder(t);
  */
  tree_clear(t);
}


int main(int argc, const char* argv[]) {
  tree_function(argc, argv);

  return 0;
}
