#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int charCount(const char* word);

int main() {

  int wordSizes[50];
  memset(wordSizes, 0, sizeof(int) * 20);

  int largestWord = 0;
  int currentWord = 0;
  int c = getchar();
  while (c != EOF) {
    if (isalpha(c) || c == '-' || c == '\'') {
      ++currentWord;
    }else if(currentWord != 0) {
      ++wordSizes[currentWord];
      if (currentWord > largestWord) {largestWord = currentWord;}
      currentWord = 0;
    }
    c = getchar();
  }
  for (int i = 1; i <= largestWord; ++i) {
    printf("%3d: ",i);
    for (int j = 0; j < wordSizes[i]; ++j) {
      printf("* ");
      if (j % 5 == 4) {
        printf("| ");
      }
    }
    printf("\n");
  }
}

int charCount(const char* word) {
  int count = 0;
  for (int i = 0; word[i] != '\0'; ++i) {
    ++count;
  }
  return count;
}
