//
//  diff_02.c
//  diff
//
//  Created by William McCarthy on 4/29/19.
//  Copyright © 2019 William McCarthy. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARGC_ERROR 1
#define TOOMANYFILES_ERROR 2
#define CONFLICTING_OUTPUT_OPTIONS 3

#define MAXSTRINGS 1024
#define MAXPARAS 4096

#define HASHLEN 200

#include "para.h"
#include "util.h"

void version(void) {
  printf("\n\n\ndiff (CSUF diffutils) 1.0.0\n");
  printf("Copyright (C) 2014 CSUF\n");
  printf("This program comes with NO WARRANTY, to the extent permitted by law.\n");
  printf("You may redistribute copies of this program\n");
  printf("under the terms of the GNU General Public License.\n");
  printf("For more information about these matters, see the file named COPYING.\n");
  printf("Written by William McCarthy, Tony Stark, and Dr. Steven Strange\n\n");
}

void todo_list(void) {
  printf("\n\n\nTODO: check line by line in a paragraph, using '|' for differences");
  printf("\nTODO: this starter code does not yet handle printing all of fin1's paragraphs.");
  printf("\nTODO: handle the rest of diff's options\n");
}

char buf[BUFLEN];
char *strings1[MAXSTRINGS], *strings2[MAXSTRINGS];
int showversion = 0, showbrief = 0, ignorecase = 0, report_identical = 0, showsidebyside = 0;
int showleftcolumn = 0, showunified = 0, showcontext = 0, suppresscommon = 0, diffnormal = 0;
int count1 = 0, count2 = 0, different = 0;


void loadfiles(const char* filename1, const char* filename2) {
  memset(buf, 0, sizeof(buf));
  memset(strings1, 0, sizeof(strings1));
  memset(strings2, 0, sizeof(strings2));
  
  FILE *fin1 = openfile(filename1, "r");
  FILE *fin2 = openfile(filename2, "r");
  
  while (!feof(fin1) && fgets(buf, BUFLEN, fin1) != NULL) { strings1[count1++] = strdup(buf); }  fclose(fin1);
  while (!feof(fin2) && fgets(buf, BUFLEN, fin2) != NULL) { strings2[count2++] = strdup(buf); }  fclose(fin2);
  
  if (count1 != count2) { different = 1;  return; }
  for (int i = 0, j = 0; i < count1 && j < count2;  ++i, ++j) {
    if (strcmp(strings1[i], strings2[j]) != 0) { different = 1;  return; }
  }
}

// Longest common substring idea borrowed from https://www.youtube.com/watch?v=ASoaQq66foQ&t=83s (Back To Back SWE)
void populate_table(int table[MAXSTRINGS][MAXSTRINGS]) {
  // Set first row to zeros
  for (int j = 0; j <= count1; ++j) {
    table[0][j] = 0;
  }
  
  // Set first column to zeros
  for (int i = 0; i <= count2; ++i) {
    table[i][0] = 0;
  }

  for (int i = 1; i <= count2; ++i) {
    for (int j = 1; j <= count1; ++j) {
      if (strcmp(strings1[j - 1], strings2[i - 1]) == 0) {
        table[i][j] = table[i - 1][j - 1] + 1;
      } else {
        table[i][j] =  table[i][j - 1] > table[i - 1][j] ? table[i][j - 1] : table[i - 1][j]; 
      }
    }
  }
}

void print_table(int table[MAXSTRINGS][MAXSTRINGS]) {
  for (int i = 0; i <= count2; ++i) {
    for (int j = 0; j <= count1; ++j) {
      printf("%3d", table[i][j]);
    }
    printf("\n");
  }
}

int reconstructfrom_table(int table[MAXSTRINGS][MAXSTRINGS],char* firstFile[], char* lcsptr[MAXSTRINGS]) {
  int lcscounterreturn;
  int i = count2;
  int j = count1;
  int lcscounter = lcscounterreturn = table[i][j] - 1;

  while (i > 0 && j > 0 && table[i][j] != 0) {
    if (table[i - 1][j]== table[i][j]) {
      --i;
    } else if (table[i][j - 1] == table[i][j]){
      --j;
    } else {
      lcsptr[lcscounter--] = firstFile[j - 1];
      --j;
      --i;
    }
  }
  return lcscounterreturn;
}

void deleteprint(int start, int end, int tomatch) {
  printf("%d,%dd%d\n", start, end, tomatch);
}

void appendprint(int toinsert, int start, int end) {
  printf("%da%d,%d\n", toinsert, start, end);
}

void changeprint(int first, int second) {
  printf("%dc%d\n", first, second);
}

void print_option(const char* name, int value) { printf("%17s: %s\n", name, yesorno(value)); }

void diff_output_conflict_error(void) {
  fprintf(stderr, "diff: conflicting output style options\n");
  fprintf(stderr, "diff: Try `diff --help' for more information.)\n");
  exit(CONFLICTING_OUTPUT_OPTIONS);
}

void setoption(const char* arg, const char* s, const char* t, int* value) {
  if ((strcmp(arg, s) == 0) || ((t != NULL && strcmp(arg, t) == 0))) {
    *value = 1;
  }
}

void showoptions(const char* file1, const char* file2) {
  printf("diff options...\n");
  print_option("diffnormal", diffnormal);
  print_option("show_version", showversion);
  print_option("show_brief", showbrief);
  print_option("ignorecase", ignorecase);
  print_option("report_identical", report_identical);
  print_option("show_sidebyside", showsidebyside);
  print_option("show_leftcolumn", showleftcolumn);
  print_option("suppresscommon", suppresscommon);
  print_option("showcontext", showcontext);
  print_option("show_unified", showunified);
  
  printf("file1: %s,  file2: %s\n\n\n", file1, file2);
  
  printline();
}


void init_options_files(int argc, const char* argv[]) {
  int cnt = 0;
  const char* files[2] = { NULL, NULL };
  
  while (argc-- > 0) {
    const char* arg = *argv;
    setoption(arg, "-v",       "--version",                  &showversion);
    setoption(arg, "-q",       "--brief",                    &showbrief);
    setoption(arg, "-i",       "--ignore-case",              &ignorecase);
    setoption(arg, "-s",       "--report-identical-files",   &report_identical);
    setoption(arg, "--normal", NULL,                         &diffnormal);
    setoption(arg, "-y",       "--side-by-side",             &showsidebyside);
    setoption(arg, "--left-column", NULL,                    &showleftcolumn);
    setoption(arg, "--suppress-common-lines", NULL,          &suppresscommon);
    setoption(arg, "-c",       "--context",                  &showcontext);
    setoption(arg, "-u",       "showunified",                &showunified);
    if (arg[0] != '-') {
      if (cnt == 2) {
        fprintf(stderr, "apologies, this version of diff only handles two files\n");
        fprintf(stderr, "Usage: ./diff [options] file1 file2\n");
        exit(TOOMANYFILES_ERROR);
      } else { files[cnt++] = arg; }
    }
    ++argv;   // DEBUG only;  move increment up to top of switch at release
  }

  if (!showcontext && !showunified && !showsidebyside && !showleftcolumn) {
    diffnormal = 1;
  }
  
  if (showversion) { version();  exit(0); }
  
  if (((showsidebyside || showleftcolumn) && (diffnormal || showcontext || showunified)) ||
      (showcontext && showunified) || (diffnormal && (showcontext || showunified))) {

    diff_output_conflict_error();
  }
  
//  showoptions(files[0], files[1]);
  loadfiles(files[0], files[1]);
  if (report_identical && showbrief) {
    if (different) {
      printf("Files %s and %s differ\n", files[0], files[1]);
      exit(0);
    } else {
      printf("Files %s and %s are identical\n", files[0], files[1]);
      exit(0);
    }
  }

  if (report_identical && !different) { printf("Files %s and %s are identical\n", files[0], files[1]);   exit(0); }
  
  if (showbrief && different) { printf("Files %s and %s differ\n", files[0], files[1]);   exit(0); }
  else if (showbrief && !different) { exit(0); }
}


int main(int argc, const char * argv[]) {
  init_options_files(--argc, ++argv);

//  para_printfile(strings1, count1, printleft);
//  para_printfile(strings2, count2, printright);
  
  int foundmatch = 0;

  if (showsidebyside) {
    para* p = para_first(strings1, count1);
    para* q = para_first(strings2, count2);
    
    para* qlast = q;
    while (p != NULL) {
    qlast = q;
    foundmatch = 0;
    while (q != NULL && (foundmatch = para_equal(p, q)) == 0) {
      q = para_next(q);
    }
    q = qlast;
  
    if (foundmatch) {
      while ((foundmatch = para_equal(p, q)) == 0) {
        para_print(q, printright);  // Prints only right
        q = para_next(q);
        qlast = q;
      }
      if (!suppresscommon) {
        para_printdiff(p, q, showleftcolumn ? printbothx : printboth); // Prints both left and right columns (side by side)
      } else {
        para_printonlydiff(p, q);
      }
      p = para_next(p);
      q = para_next(q);
    } else {
      para_print(p, printleft);
      p = para_next(p);
    }
  }
  while (q != NULL) {
    para_print(q, printright);
    q = para_next(q);
  }
  } else if (diffnormal) {
    // Create a table to calculate the longest subsequence length
    int table[MAXSTRINGS][MAXSTRINGS];
    char* lcs[MAXSTRINGS];
    populate_table(table); 
    // print_table(table);
    int lcsnum = reconstructfrom_table(table, strings1, lcs);
    int lcsplaceholder = 0;
    
    int firstStart = 0, firstEnd = 0, secondStart = 0, secondEnd = 0;
    while (firstStart != count1 && secondStart != count2) {
      // Find strings that require deletions
      int firstTemp = firstStart;
      while (strcmp(strings1[firstTemp], lcs[lcsplaceholder]) != 0) {
        ++firstEnd;
        ++firstTemp;
      }
      if (firstEnd - firstStart > 1) { 
        deleteprint(firstStart + 1, firstEnd, secondStart); 
        for (int i = firstStart; i < firstEnd; ++i) {
          lprint(strings1[i]);
        }
      }
      // Find strings that require appending
      int secondTemp = secondStart;
      while (strcmp(strings2[secondTemp], lcs[lcsplaceholder]) != 0) {
        ++secondEnd;
        ++secondTemp;
      }
      if (secondEnd - secondStart > 1) { 
        appendprint(firstStart, secondStart + 1, secondEnd);
        for (int i = secondStart; i < secondEnd; ++i) {
          rprint(strings2[i]);
        }
      } else if (firstEnd - firstStart == 1 && secondEnd - secondStart == 1) {
        changeprint(firstStart + 1, secondStart + 1);
        lprint(strings1[firstStart]);
        printf("---\n");
        rprint(strings2[secondStart]);
      }
      firstStart = ++firstEnd;
      secondStart = ++secondEnd;

      ++lcsplaceholder;
    }
  
    while (firstEnd != count1) {
      ++firstEnd;
    }
    if (firstEnd - firstStart > 0) {
      deleteprint(firstStart, firstEnd - 1, secondStart - 1);
      for (int i = firstStart - 1; i < firstEnd - 1; ++i) {
        lprint(strings1[i]);
      }
    }

    while(secondEnd != count2) {
      ++secondEnd;
    }

    if (secondEnd - secondStart > 0) {
      appendprint(firstStart - 1, secondStart, secondEnd - 1);
      for (int i = secondStart - 1; i < secondEnd - 1; ++i) {
        rprint(strings2[i]);
      }
    }
  }
  

  return 0;
}
