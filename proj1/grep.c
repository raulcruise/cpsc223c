#include "grep.h"

int main(int argc, char *argv[]) {

	// Declare required local variables and check correct usage
	int err = 0;
	if (argc == 1) {
		printf("Usage: grep [OPTION]... PATTERN [FILE]...\n");
		err = 1;
		return err;
	}
	int returnval = 1;
	char line[1000];
	FILE* file;

	// Compile the regexp
	compile(argv[1]);
	countbuf();

	// If argc == 2, take input from stdin
	if (argc == 2) {
		while(fgets(line, sizeof(line), stdin) != NULL) { // Pull input until EOF
			if (execute(line)) { 								// Execute searches for regexp in line
				print(line);											// Line is printed if execute
				returnval = 0;										// Return value is set to 0 if execute
			}
			memset(line, 0, sizeof(line));
		}
	} else { // if argc > 2, then open files and take input from them in order
		for(int i = 2; i < argc; ++i) {
			file = fopen(argv[i], "r");
			if (file) {
				while(fgets(line, sizeof(line), file) != NULL) {
					if (execute(line)) {
						if (argc > 3) {
							printfilename(argv[i]);
						}
						print(line);
						returnval = 0;
					}
					memset(line, 0, sizeof(line));
				}
			} else { // If file could not be opened, print error and set return value
				fprintf(stderr, "error: could not open file: %s\n", argv[i]);
				err = 2;
			}
		}
	} // If a file wasn't opened correctly, return 2, else return 0 if a match
	return err ? err : returnval; 	// was found, else 1
}

void compile(char* regex) {
	char *ep, *lastep, *bracketp, bracket[NBRA];
	int cclcnt;

	ep = expbuf;
	bracketp = bracket;
	if (*regex == '\n') {
		peekc = *regex;
		*regex = '\0';
	}
	if (*regex == '\0') {
		if (*ep==0)
			error(Q);
		return;
	}
	nbra = 0;
	if (*regex=='^') {
		++regex;
		*ep++ = CCIRC;
	}
	peekc = *regex;
	lastep = 0;
	--regex;
	for (;;) {
		if (ep >= &expbuf[ESIZE])
			onerror();
		++regex;
		if (*regex == '\n') {
			peekc = *regex;
			*regex = '\0';
		}
		if (*regex=='\0') {
			if (bracketp != bracket)
				onerror();
			*ep++ = CEOF;
			return;
		}
		if (*regex!='*')
			lastep = ep;
		switch (*regex) {

		case '\\':
			if (*++regex=='(') {
				if (nbra >= NBRA)
					onerror();
				*bracketp++ = nbra;
				*ep++ = CBRA;
				*ep++ = nbra++;
				continue;
			}
			if (*regex == ')') {
				if (bracketp <= bracket)
					onerror();
				*ep++ = CKET;
				*ep++ = *--bracketp;
				continue;
			}
			if (*regex>='1' && *regex<'1'+NBRA) {
				*ep++ = CBACK;
				*ep++ = *regex-'1';
				continue;
			}
			*ep++ = CCHR;
			if (*regex=='\n')
				onerror();
			*ep++ = *regex;
			continue;

		case '.':
			*ep++ = CDOT;
			continue;

		case '\n':
			onerror();

		case '*':
			if (lastep==0 || *lastep==CBRA || *lastep==CKET) {
				*ep++ = CCHR;
				*ep++ = *regex;
			} else {
				*lastep |= STAR;
			}
			continue;

		case '$':
			if ((peekc=*++regex) != '\0' && peekc!='\n') {
				*ep++ = CCHR;
				*ep++ = *regex;
			} else {
				*ep++ = CDOL;
				--regex;
			}
			continue;

		case '[':
			*ep++ = CCL;
			*ep++ = 0;
			cclcnt = 1;
			if (*++regex == '^') {
				++regex;
				ep[-2] = NCCL;
			}
			do {
				if (*regex=='\n')
					onerror();
				if (*regex=='-' && ep[-1]!=0) {
					if (*++regex==']') {
						*ep++ = '-';
						cclcnt++;
						break;
					}
					while (ep[-1]<*regex) {
						*ep = ep[-1]+1;
						ep++;
						cclcnt++;
						if (ep>=&expbuf[ESIZE])
							onerror();
					}
				}
				*ep++ = *regex;
				cclcnt++;
				if (ep >= &expbuf[ESIZE])
					onerror();
			} while (*++regex != ']');
			lastep[1] = cclcnt;
			continue;

		default:
			*ep++ = CCHR;
			*ep++ = *regex;
		}
	}
}

void onerror(void) {
	fprintf(stderr, "grep: Invalid regular expression\n");
	expbuf[0] = 0;
	nbra = 0;
	exit(2);
}

int execute(char* line) {
	char *p1, *p2;
	int c;

	for (c=0; c<NBRA; c++) {
		braslist[c] = 0;
		braelist[c] = 0;
	}
	p2 = expbuf;
	p1 = line;
	if (*p2==CCIRC) {
		return(advance(p1, p2+1));
	}
	/* fast check for first character */
	if (*p2==CCHR) {
		c = p2[1];
		do {
			if (*p1!=c)
				continue;
			if (advance(p1, p2)) {
				return(1);
			}
		} while (*p1++);
		return(0);
	}
	/* regular algorithm */
	do {
		if (advance(p1, p2)) {
			return(1);
		}
	} while (*p1++);
	return(0);
}

int advance(char *lp, char *ep) {
	char *curlp;
	int i;

	for (;;) switch (*ep++) {

	case CCHR:
		if (*ep++ == *lp++)
			continue;
		return(0);

	case CDOT:
		if (*lp++)
			continue;
		return(0);

	case CDOL:
		if (*lp=='\n')
			continue;
		return(0);

	case CEOF:
		startred = lp - bufcount;
		return(1);

	case CCL:
		if (cclass(ep, *lp++, 1)) {
			ep += *ep;
			continue;
		}
		return(0);

	case NCCL:
		if (cclass(ep, *lp++, 0)) {
			ep += *ep;
			continue;
		}
		return(0);

	case CBRA:
		braslist[*ep++] = lp;
		continue;

	case CKET:
		braelist[*ep++] = lp;
		continue;

	case CBACK:
		if (braelist[i = *ep++]==0)
			error(Q);
		if (backref(i, lp)) {
			lp += braelist[i] - braslist[i];
			continue;
		}
		return(0);

	case CBACK|STAR:
		if (braelist[i = *ep++] == 0)
			error(Q);
		curlp = lp;
		while (backref(i, lp))
			lp += braelist[i] - braslist[i];
		while (lp >= curlp) {
			if (advance(lp, ep))
				return(1);
			lp -= braelist[i] - braslist[i];
		}
		continue;

	case CDOT|STAR:
		curlp = lp;
		while (*lp++)
			;
			do {
				lp--;
				if (advance(lp, ep))
					return(1);
			} while (lp > curlp);
			return(0);

	case CCHR|STAR:
		curlp = lp;
		while (*lp++ == *ep)
			;
		ep++;
		do {
			lp--;
			if (advance(lp, ep))
				return(1);
		} while (lp > curlp);
		return(0);

	case CCL|STAR:
	case NCCL|STAR:
		curlp = lp;
		while (cclass(ep, *lp++, ep[-1]==(CCL|STAR)))
			;
		ep += *ep;
		do {
			lp--;
			if (advance(lp, ep))
				return(1);
		} while (lp > curlp);
		return(0);

	default:
		error(Q);
	}
}

int backref(int i, char *lp) {
	char *bp;

	bp = braslist[i];
	while (*bp++ == *lp++)
		if (bp >= braelist[i])
			return(1);
	return(0);
}

int cclass(char *set, int c, int af) {
	int n;

	if (c==0)
		return(0);
	n = *set++;
	while (--n)
		if (*set++ == c)
			return(af);
	return(!af);
}

void countbuf(void) { 	// Counts the characters that will be red
	for (int i = 0; i < ESIZE + 4; ++i) {
		if (expbuf[i] == CCHR || expbuf[i] == CCL){
			++bufcount;
		}
	}
}

void print(char* line) { 			// Prints line with the regexp that matched
	 while (line != startred){ 	// with the MATCHING regexp in red
		 printf("%c", *line++);
	 }
	 red();
	 for (int i = 0; i < bufcount; ++i) {
		 printf("%c", *line++);
	 }
	 reset();
	 do {
		 printf("%c", *line);
	 } while(*line++ != '\n');
}

void printfilename(char* filename) {	// Prints file name identically as grep
	magenta();
	printf("%s", filename);
	cyan();
	printf(":");
	reset();
}

void cyan(void) { printf("\033[0;36m"); }

void magenta(void) { printf("\033[0;35m"); }

void red(void) { printf("\033[1;31m"); }

void reset(void) { printf("\033[0m"); }
