#include <signal.h>
#include <stdio.h>
#include <string.h>

#define	ESIZE	256
#define	NBRA	5

#define	CBRA	1
#define	CCHR	2
#define	CDOT	4
#define	CCL	6
#define	NCCL	8
#define	CDOL	10
#define	CEOF	11
#define	CKET	12
#define	CBACK	14
#define	CCIRC	15

#define	STAR	01

char	Q[]	= "";

int peekc;
char	expbuf[ESIZE+4];
char  *startred;


char	*braslist[NBRA];
char	*braelist[NBRA];
int	nbra;

int advance(char *lp, char *ep);
int backref(int i, char *lp);
void countbuf(void);
int cclass(char *set, int c, int af);
void compile(char* regex);
void cyan(void);
void error(char *s);
int execute(char* line);
void magenta(void);
void onerror(void);
void print(char* line);
void printfilename(char* filename);
void red(void);
void reset(void);
