#include <stdio.h>
#define MAXLENGTH 1000

int getLine(char line[], int maxline);
void copy(char to[], char from[]);

int main() {
  int curr;
  char line[MAXLENGTH];
  while ((curr = getLine(line, MAXLENGTH)) > 0) {
    while (line[curr - 2] == ' ' || line[curr - 2] == '\t') {
      --curr;
    }
    line[curr - 1] = '\n';
    line[curr] = '\0';
    printf("%s", line);
  }
    return 0;
}

int getLine(char line[], int maxline) {
  int character, size;

  for (size = 0; size < maxline - 1 && (character = getchar()) != EOF && character != '\n'; ++size){
    line[size] = character;
  }
  if (character == '\n') {
    line[size] = character;
    ++size;
  }
  line[size] = '\0';
  return size;
}

void copy(char to[], char from[]) {
  int i = 0;
  while ((to[i] = from[i]) != '\0') {
    ++i;
  }
}
