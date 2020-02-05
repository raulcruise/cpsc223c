#include "stdbool.h"
#include "stdio.h"
bool openfiles(int argc, const char* argv[], FILE** pfin, FILE** pfout);
void closeFiles(FILE* fin, FILE* fout);
