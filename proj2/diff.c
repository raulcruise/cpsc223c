//
//  diff_02.c
//  diff
//
//  Created by William McCarthy on 4/29/19.
//  Copyright © 2019 William McCarthy. All rights reserved.
//
#include "diff.h"

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