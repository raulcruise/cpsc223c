#include <stdio.h>
#include <stdlib.h>
#include "fileop.h"

int main(int argc, const char* argv[]) {
  if (argc != 3 && argc != 4) {
    printf("Usage: ./entab infile outfile n");
    exit(1);
  }

  FILE* fin;
  FILE* fout;
  int tabBreaks, spaceNum;
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

  while (c != EOF) {
    if (c != ' ' && c != '\n') { // If the character is not a space or newline
      fputc(c,fout);             // then simply copy over the character and
      ++counter;                 // add 1 to our line character "counter" variable
    } else if (c == ' ') {       // If the character is a space
      ++spaceNum;                // Add one to the line space counter
      c = fgetc(fin);            // and get the next character
      while (c == ' ') {         // keep checking if the consecutive characters
        ++spaceNum;              // are spaces until our equation determines
        if (spaceNum == tabBreaks - (counter % tabBreaks)) { // that a tab
          fputc('\t', fout);     // should be placed instead of "n" spaces
          spaceNum = 0;          // If a tab is input, set our space counter and
          counter = 0;           // character counter to 0
        }
        c = fgetc(fin);
      }
      for (int i = 0; i < spaceNum; ++i) {
        fputc(' ', fout);
      }
      counter += spaceNum;
      spaceNum = 0;
      if (c != ' ') { fputc(c,fout); ++counter; }
    } else if (c == '\n'){
      fputc(c, fout);
      counter = 0;
      spaceNum = 0;
    }
    c = fgetc(fin);
  }
  closeFiles(fin, fout);
}
