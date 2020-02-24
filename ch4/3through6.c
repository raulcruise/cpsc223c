//
//  main.c
//  rpn
//
//  Created by William McCarthy on 192//20.
//  Copyright © 2020 William McCarthy. All rights reserved.
//

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXOP   100    /* max size of operand/operator */
#define NUMBER '0'     /* signal that a number was found */
#define MAXVAL  100
#define SINOP '1'
#define EXPOP '2'
#define POWOP '3'
#define ANS   '4'
#define SETVAR '5'
#define GETVAR '6'

size_t sp = 0;   // aka unsigned long -- printf using %zu
double val[MAXVAL];   // stack of values

char buf[BUFSIZ];
size_t bufp = 0;

int getch_(void) { return (bufp > 0) ? buf[--bufp] : getchar(); }
void ungetch_(int c) {
  if (bufp >= BUFSIZ) { fprintf(stderr, "ungetch: too many characters\n");  return; }
  buf[bufp++] = c;
}

int getop(char* s) {
  int i, c;
  while ((s[0] = c = getch_()) == ' ' || c == '\t') { }  // skip whitespace
  s[1] = '\0';


  switch(c) {
    case 'a': if (getch_() == 'n' && getch_() == 's') {return ANS;}
      break;
    case 's': if (getch_() == 'i' && getch_() == 'n') {return SINOP;}
      break;
    case 'e': if (getch_() == 'x' && getch_() == 'p') {return EXPOP;}
      break;
    case 'p': if (getch_() == 'o' && getch_() == 'w') {return POWOP;}
      break;
  }

  if (c >= 'a' && c <= 'z') {
    return GETVAR;
  }else if (c >= 'A' && c <= 'Z') {
    return SETVAR;
  }

  if (!isdigit(c) && c != '.' && c != '-') { return c; }  // if not a digit, return

  i = 0;

  if (c == '-') {
    if (isdigit(c = getch_()) || c == '.') { // Take provision for negative numbers including those starting with a decimal
      s[++i] = c;
    } else {
      if (c != EOF) {
        ungetch_(c);
      }
      return '-';
    }
  }

  if (isdigit(c)) {  // get digits before '.'
    while (isdigit(s[++i] = c = getch_())) { }
  }
  if (c == '.') {    // get digits after decimal (if any)
    while (isdigit(s[++i] = c = getch_())) { }
  }
  s[i] = '\0';
  if (c != EOF) { ungetch_(c); }
  return NUMBER;      // return type is NUMBER, number stored in s
}

double pop(void) {
  if (sp == 0) { fprintf(stderr, "stack underflow\n");  return 0.0; }
  return val[--sp];
}

void push(double f) {
  if (sp == MAXVAL) { fprintf(stderr, "stack overflow -- can't push %g\n", f);  return; }
  val[sp++] = f;
}

void clear(void) {
  sp = 0;
  memset(val, 0, MAXVAL * sizeof(double));
}

void rpn(void) {
  int type;
  double op1, op2, ans;
  double vars[26];

  for (int i = 0; i < 26; ++i) {
    vars[i] = 0.0;
  }

  char s[BUFSIZ];

  while ((type = getop(s)) != EOF) {
    switch(type) {
      case '\n':    printf("\t%.8g\n", ans = pop());  break;
      case ANS:     push(ans);                  break;
      case NUMBER:  push(atof(s));              break;
      case SINOP:   push(sin(pop()));           break;
      case EXPOP:   push(exp(pop()));           break;
      case POWOP:   op2 = pop(); push(pow(pop(), op2)); break;
      case SETVAR: // If a capital letter is recieved, we'll assign the previous value to that variable
        vars[s[0] - 'A'] = op1 = pop(); push(op1);
        break;
      case GETVAR: // In the case that a lower case is recieved, we'll treat it as it's variable value
        push(vars[s[0] - 'a']);
        break;
      case '?': // Use question mark to check top
        op2 = pop();
        printf("\t%.8g\n", op2);
        push(op2);
        break;
      case '_': // Use underscore to clear
        clear();
        break;
      case '&': // Use and to double
        op2 = pop();
        push(op2);
        push(op2);
        break;
      case '<': // Use left arrow to swap
        op1 = pop();
        op2 = pop();
        push(op1);
        push(op2);
        break;
      case '+':     push(pop() + pop());        break;
      case '*':     push(pop() * pop());        break;
      case '-':     push(-(pop() - pop()));     break;
      case '/':
        if ((op2 = pop()) == 0.0) { fprintf(stderr, "divide by zero\n");  break; }
        push(pop() / op2);
        break;
      case '%':
        if ((op2 = pop()) == 0.0) { fprintf(stderr, "mod by zero\n"); break; }
        push(fmod(pop(), op2));
        break;
      default:
        fprintf(stderr, "unknown variable %s\n", s);  break;
    }
  }
}

int main(int argc, const char * argv[]) {
  rpn();

  return 0;
}
