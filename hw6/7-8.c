#include <stdio.h>
#define BUFSIZE 100

int main(int argc, char* argv[]) {
  FILE* input;
  char line[BUFSIZE];
  for (int i = 1; i < argc; ++i) {
    input = fopen(argv[i], "r");
    if (!input) {
      fprintf(stderr, "error: no such file name %s", argv[i]);
      return 1;
    }
    printf("\t\t\t|||%s|||\n", argv[i]);
    int lineNum = 0, fpage = 1;
    while (fgets(line, BUFSIZE, input) != NULL) {
      ++lineNum;
      printf("%s", line);
      if (lineNum % 10 == 0) {
        printf("\tPage %d\n",fpage++);
        printf ("-------------------------------------------------------------\n");
      }
    }
    while (lineNum % 10 != 0) {
      ++lineNum;
      printf("\n");
      if (lineNum % 10 == 0) {
        printf("\tPage %d\n",fpage);
        printf ("-------------------------------------------------------------\n");
      }
    }
  }
}
