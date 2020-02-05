#include <stdio.h>
#include <stdlib.h>
#include "fileop.h"

int main(int argc, const char* argv[]) {
  if (argc != 3 && argc != 4) {
    printf("Usage: ./fold infile outfile n");
    exit(1);
  }

  FILE* fin;
  FILE* fout;
  char characters[1000];
  int foldNum, alreadyWritten = 0, lastSpace = 0;
  if (argc == 4) {
    foldNum = atoi(argv[3]);
  } else {
    foldNum = 50;
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
    characters[counter] = c;
    if (c == ' ' || c == '\t') { lastSpace = counter; }
    if (c == '\n') {
      for (int i = 0; i <= counter; ++i) {
        fputc(characters[i], fout);
      }
      counter = -1;
      lastSpace = 0;
    }

    if (counter % foldNum == foldNum - 1 - alreadyWritten) {
      if (lastSpace == 0){
        for (int i = 0; i < foldNum; ++i) {
          fputc(characters[i], fout);
        }
        fputc('\n', fout);
        counter = -1;
        lastSpace = 0;
      }else{
        for (int i = 0; i < lastSpace; ++i) {
          fputc(characters[i], fout);
        }
        fputc('\n', fout);
        alreadyWritten = 0;
        for (int i = lastSpace + 1; i <= counter; ++i) {
          fputc(characters[i], fout);
          alreadyWritten++;
        }
        counter = -1;
        lastSpace = 0;
      }
    }

    c = fgetc(fin);
    ++counter;
  }
  closeFiles(fin, fout);
}
