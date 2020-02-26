///
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
#define MAXVAL  100
#define NUMBER '0'     /* signal that a number was found */
#define MATH   '1'
#define STACK  '3'
#define ANS    '4'
#define VARIABLE '5'

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

  if (c == '#') {
    while (isalpha(s[++i] = c = getch_()));
    s[i] = '\0';
    if (c != EOF ) {ungetch_(c);}
    return STACK;
  }

  if (isalpha(c)) {
    while (isalpha(s[++i] = c = getch_()));
    s[i] = '\0';
    if (c != EOF ) {ungetch_(c);}
    if (strcmp(s, "ans") == 0) { return ANS; }
    if ( i > 2) {
      return MATH;
    }
  }

  if (c == '=' || c == '?') {
    while (isalpha(s[++i] = c = getch_())) {}
    s[i] = '\0';
    return VARIABLE;
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

void math(char* s) {
  double op1, op2, result = 0;
  if (strcmp(s, "sin") == 0) {
    result = sin(pop());
  } else if (strcmp(s, "cos")) {
    result = cos(pop());
  } else if (strcmp(s, "tan")) {
    result = tan(pop());
  } else if (strcmp(s, "pow")) {
    op2 =  pop();
    result = pow(pop(), op2);
  } else if (strcmp(s, "exp")) {
    result = exp(pop());
  } else {
    fprintf(stderr, "unknown math operation\n");
  }
  push(result);
}

void stack(char* s) {
  double op1, op2, result = 0;
  if (strcmp(s, "#dup") == 0) {
    op2 = pop();
    push(op2);
    push(op2);
  } else if (strcmp(s, "#top") == 0) {
    op2 = pop();
    printf("\t%.8g\n", op2);
    push(op2);
  } else if (strcmp(s, "#clear") == 0) {
    clear();
  } else if (strcmp(s, "#swap") == 0) {
      op1 = pop();
      op2 = pop();
      push(op1);
      push(op2);
  } else {
    fprintf(stderr, "unknown stack operation\n");
  }
}

void variables(char *s) { // 5 =a to assign a variable value
  if (*s == '=') {        // ?a   to extract variable value
    val[*++s - 'a'] = pop();
  } else if (*s == '?') {
    push(val[*++s - 'a']);
  }
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
      case MATH:    math(s);                    break;
      case STACK:   stack(s);                   break;
      case VARIABLE: variables(s);              break;
      case '+':     push(pop() + pop());        break;
      case '*':     push(pop() * pop());        break;
      case '-':     push(-(pop() - pop()));     break;
      case '/':
        if ((op2 = pop()) == 0.0) { fprintf(stderr, "error: divide by zero\n");  break; }
        push(pop() / op2);
        break;
      case '%':
        if ((op2 = pop()) == 0.0) { fprintf(stderr, "error: mod by zero\n"); break; }
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
