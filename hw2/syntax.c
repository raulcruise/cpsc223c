#include <stdio.h>
#include <stdlib.h>
#include "fileop.h"

int main(int argc, const char* argv[]) {
  if (argc != 2) {
    printf("Usage: ./syntax file\n");
    exit(1);
  }

  int curly = 0, bracket = 0, paren = 0, squote = 0, dquote = 0;
  FILE* fin;
  fin = fopen(argv[1], "r");

  char c = getc(fin);

  while (c != EOF) {

    // Skip strings dQuotes
    if (c == '\"') {
      ++dquote;
      c = getc(fin);
      while (c != '\"' && c != EOF) {
        if (c == '\\') {
          c = getc(fin);
        }
        c = getc(fin);
      }
    }
    if (c == '\'') {
      ++squote;
      c = getc(fin);
      while (c != '\'') {
        if (c == '\\') {
          c = getc(fin);
        }
        c = getc(fin);
      }
    }

    if (c == '/') {
      c = fgetc(fin);
      if (c == '/') {
        while (c != '\n') {
          c = fgetc(fin);
        }
      } else if (c == '*') {
        int starBefore;
        do {
          starBefore = 0;
          if (c == '*') { starBefore = 1;}
          c = fgetc(fin);
        } while (!( c == '/' && starBefore == 1));
      }
    }

    if (dquote % 2 != 0 || squote % 2 != 0) {
      if (c == '\\') {
        c = getc(fin);
      }
    }
    if (true) {
      if (c == '(') ++paren;
      if (c == '{') ++curly;
      if (c == '[') { ++bracket;}
      if (c == '\'') ++squote;
      if (c == '\"') ++dquote;
      if (c == ')') --paren;
      if (c == '}') --curly;
      if (c == ']'){ --bracket;}
    }
    c = getc(fin);
  }
  if (paren != 0) {printf("Error: Umatched parenthesis\n");}
  if (curly != 0) {printf("Error: Umatched curly brace\n");}
  if (bracket != 0) {printf("Error: Umatched bracket\n");}
  if (0 != squote % 2) {printf("Error: Unmatched single quote\n");}
  if (0 != dquote % 2) {printf("Error: Unmatched quotes\n");}

}
