#include <stdio.h>
#define MAXLENGTH 1000

int getLine(char line[], int maxline);
void copy(char to[], char from[]);

int main() {
  int curr, max;
  char line[MAXLENGTH];
  char longest[MAXLENGTH];
  char firstRead[MAXLENGTH];

  max = 0;
  while ((curr = getLine(line, MAXLENGTH)) > 0) {
    copy(firstRead,line); /* Store the first line read in case we need to return this as the longest */
    while (curr != 0 && curr % 999 == 0 && line[curr % MAXLENGTH] != '\n') { /* While the line buffer is full and it's last character isn't a newline */
      curr += getLine(line, MAXLENGTH);                                      /* add the string size to curr */
    }
    if (curr > max) { /* If our curr count is great than the max recorded, redefine the max as curr's value */
      max = curr;
      copy(longest, firstRead);
    }
  }
  if (max > 0) { /* if there's a "longest line", print the statistics */
    printf("Length: %d\n", max);
    printf("%s", longest);
    return 0;
  }
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
