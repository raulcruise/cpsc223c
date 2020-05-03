#include <stdio.h>
#include <string.h>
#define BUFSIZE 100
int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("usage: ./7-6 fileone filetwo\n");
    return 1;
  }
  FILE* first = fopen(argv[1], "r");
  if (!first) {
    fprintf(stderr, "error: no such file name %s", argv[1]);
    return 2;
  }
  FILE* second = fopen(argv[2], "r");
  if (!second) {
    fprintf(stderr, "error: no such file name %s", argv[2]);
    return 2;
  }

  char line[BUFSIZE];
  char sline[BUFSIZE];
  int lineNum = 0;
  char *a, *b;
  do {
    a = fgets(line, BUFSIZE, first);
    b = fgets(sline, BUFSIZE, second);
    ++lineNum;
    if (strcmp(line, sline) != 0) {
      printf("Line: %d\n", lineNum);
      printf("%s: %s\n", argv[1], line);
      printf("%s: %s\n", argv[2], sline);
      return 0;
    }
  } while(a || b);
  printf("Files are identical.\n");
  return 0;
}
