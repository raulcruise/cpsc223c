#include "fileop.h"

bool openfiles(int argc, const char* argv[], FILE** pfin, FILE** pfout) {
  if ((*pfin = fopen(argv[1], "r")) && (*pfout = fopen(argv[2], "w"))) {
    return true;
  } else {
    return false;
  }
}

void closeFiles(FILE* fin, FILE* fout) {
  fclose(fin);
  fclose(fout);
}
