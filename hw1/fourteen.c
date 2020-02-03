#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHARACTERNUM 127

int main() {

  int charCount[CHARACTERNUM];
  memset(charCount, 0, sizeof(int) * CHARACTERNUM);

  int c = getchar();
  while (c != EOF) {
    if (c > 32 && c < CHARACTERNUM){
      ++charCount[c];
    }
    c = getchar();
  }

  for (int i = 33; i < CHARACTERNUM; ++i) {
    if (charCount[i] != 0 && charCount[i] != '\n') {
      printf("%3c: ",i);
      for (int j = 0; j < charCount[i]; ++j) {
        printf("* ");
        if (j % 5 == 4) {
          printf("| ");
        }
      }
      printf("\n");
    }
  }
}
