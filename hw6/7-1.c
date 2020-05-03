#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char* argv[]) {

  // Function pointer
  int (*execute)(int);

  int prog = 0;
  if (strcmp(argv[0], "./toup") == 0) { execute = &toupper; }
  else if (strcmp(argv[0], "./tolower") == 0) { execute = &tolower; }
  else {
    fprintf(stderr, "error: save executable files as toup & tolower\n");
    return 1;
  }
  int c;
  while ((c = fgetc(stdin)) != EOF) {
    putc(execute(c), stdout);
  }
  return 0;
}
