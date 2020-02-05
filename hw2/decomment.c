#include <stdio.h>
#include <stdlib.h>
#include "fileop.h"

int main(int argc, const char* argv[]) {
  if (argc != 3) {
    printf("Usage: ./decomments infile outfile");
    exit(1);
  }

  int inQuotes = 0;
  FILE* fin;
  FILE* fout;

  if (!openfiles(argc, argv, &fin, &fout)) {
    printf("ERROR: file does not exist\n");
    exit(2);
  }

  char c = fgetc(fin);
  while (c != EOF) {
    if (inQuotes && c == '\\') {
      fputc(c, fout);
      c = fgetc(fin);
      fputc(c, fout);
    }else if (c != '\'' && c != '/' && c != '\"') {
      fputc(c, fout);
    }else if (c == '\'' || c == '\"') {
      fputc(c, fout);
      if (inQuotes == 1) {
        inQuotes = 0;
      } else { inQuotes = 1; }
    } else if (!inQuotes && c == '/') {
      c = fgetc(fin);
      if (c == '/') {
        while (c != '\n') {
          c = fgetc(fin);
        }
        fputc(c, fout);
      } else if (c == '*') {
        do {
          c = fgetc(fin);
        } while (c != '*' && '/' != fgetc(fin));
      } else {
        fputc('/', fout);
        fputc(c, fout);
      }
    }
    c = fgetc(fin);
  }
  closeFiles(fin, fout);
}
