#include <stdio.h>
#include <stdlib.h>
#include "fileop.h"

int main(int argc, const char* argv[]) {
  if (argc != 3 && argc != 4) {
    printf("Usage: ./detab infile outfile n");
    exit(1);
  }

  FILE* fin;
  FILE* fout;
  int tabBreaks;
  if (argc == 4) {
    tabBreaks = atoi(argv[3]);
  } else {
    tabBreaks = 10;
  }
  if (!openfiles(argc, argv, &fin, &fout)) {
    printf("ERROR: file does not exist\n");
    exit(2);
  }

  // Initialize counter to count how many characters have been read since the
  // last user tab or since the start of the file
  int counter = 0;
  char c = fgetc(fin);

  ++counter;

  // While the EOF isn't reached
  while (c != EOF) {
    // If the characte read is a tab
    if (c == '\t') {
      // Print out as many spaces as it will take to reach the next tab break
      for (int i = 0; i < tabBreaks - (counter - 1 % tabBreaks); ++i) {
        fputc(' ', fout);
      }
      // Reset character counter
      counter = 0;
    } else {                // If the character is not a newline,
      fputc(c, fout);       // simply output the character
    }
    if (c == '\n') { counter = 0;} // Reset the character counter every newline
    c = fgetc(fin); // Grab new character
    ++counter;      // and increase the counter
  }
  closeFiles(fin, fout);
  return 0;
}
