//#include <stdio.h>
#include <dirent.h>
#include "grep.h"

typedef struct dirent dirent;
DIR* dir;
dirent *in_file;

int mfiles = 0;
char* files[];
char* pattern;
char* fname;
int nfiles = 0;

int main(int argc, char *argv[]) {

	char *p1, *p2;
	if(argc <= 2){
		return 0;
	}
	argv++;
	pattern = *argv++;
	fname = *argv;
	dir = opendir(fname);

	//if directory: (grep hello /documents/grep )
	if(dir == NULL){
		closedir(dir);
	}
	else{
		while((in_file = readdir(dir)) != NULL){
			files[nfiles++] = in_file->d_name;
			//puts(files[nfiles - 1], 0);
		}
		if(nfiles > 0){
			mfiles = 1;
			puts("multiple files!\n", 0);
		}
	}

	//if listed multiple files: (grep hello hello.txt yes.txt)
	if(argc > 3){
		mfiles = 1;
		puts("multiple files!\n", 0);
	}
	if(nfiles == 0){
		files[0] = *argv;
		nfiles = 1;
	}
	// p1 = *argv;
	// p2 = savedfile;
	// while ((*p2++ = *p1++))
	// 	if (p2 >= &savedfile[sizeof(savedfile)])
	// 		p2--;
	// globp = "r";


	for(int i = 0; i < nfiles; i++){
		//p1 = *argv;
		p1 = files[i];
		p2 = savedfile;
		while ((*p2++ = *p1++))
			if (p2 >= &savedfile[sizeof(savedfile)])
				p2--;
		globp = "r";
		zero = (unsigned *)malloc(nlall*sizeof(unsigned));//need
		init();
		//listn++;
		commands();
	}
	//if there are multiple files
	// do {
	//	fname = next filename
	// } while(mfiles);

	// zero = (unsigned *)malloc(nlall*sizeof(unsigned));//need
	// init();
	// //listn++;
	// commands();
	return 0;
}

void commands(void) {
	unsigned int *a1;
	int c;
	int temp;
	char lastsep;

	for (;;) {
	c = '\n';
	for (addr1 = 0;;) {
		lastsep = c;
		a1 = address();
		c = getchr();
		if (c!=',' && c!=';')
			break;
		if (a1==0) {
			a1 = zero+1;
			if (a1>dol)
				a1--;
		}
		addr1 = a1;
		if (c==';')
			dot = a1;
	}
	if ((addr2=a1)==0) {
		given = 0;
		addr2 = dot;
	}
	else
		given = 1;
	if (addr1==0)
		addr1 = addr2;
	switch(c) {
		//IMPORTANT!!!!
		case 'g':

			global(1);
			continue;

		case 'l':
			listf++;
		case 'p':
		case 'P':
			newline();
			print();
			continue;

		//IMPORTANT!!!
		case 'r':
			filename(c);
		caseread:
			if ((io = open(file, 0)) < 0){
				//IF FILE OPEN ERROR PRINT SOMETHING
			}
			c = zero != dol;
			append(getfile, addr2);
			continue;

		case EOF:
			return;
		}
	}
}

void print(void) {
	unsigned int *a1;

	a1 = addr1;
	do {
		if(mfiles){
			puts(fname, mfiles);
		}
		if (listn) {
			count = a1-zero;

		}
		puts(getline(*a1++),0);
	} while (a1 <= addr2);
	dot = addr2;
	listf = 0;
	//listn = 0;
	pflag = 0;
}

unsigned int *address(void) {
	int sign;
	unsigned int *a, *b;
	int opcnt, nextopand;
	int c;

	nextopand = -1;
	sign = 1;
	opcnt = 0;
	a = dot;
	do {
		do c = getchr(); while (c==' ' || c=='\t');
		switch (c) {
		default:
			if (nextopand == opcnt) {
				a += sign;
				if (a<zero || dol<a)
					continue;
			}
			if (c!='+' && c!='-' && c!='^') {
				peekc = c;
				if (opcnt==0)
					a = 0;
				return (a);
			}
			sign = 1;
			if (c!='+')
				sign = -sign;
			nextopand = ++opcnt;
			continue;
		}
		sign = 1;
		opcnt++;
	} while (zero<=a && a<=dol);
	/*NOTREACHED*/
	return 0;
}

//need
void setwide(void) {
	if (!given) {
		addr1 = zero + (dol>zero);
		addr2 = dol;
	}
}

//need
void newline(void) {
	int c;

	if ((c = getchr()) == '\n' || c == EOF)
		return;
	if (c=='p' || c=='l' || c=='n') {
		pflag++;
		if (c=='l')
			listf++;
		else if (c=='n')
			listn++;
		if ((c=getchr())=='\n')
			return;
	}
}

//need
void filename(int comm) {
	char *p1, *p2;
	int c;

	count = 0;
	c = getchr();
	if (c=='\n' || c==EOF) {
		p1 = savedfile;
		p2 = file;
		while ((*p2++ = *p1++))
			;
		return;
	}
	while ((c = getchr()) == ' ')
		;
	p1 = file;
	do {
		*p1++ = c;
	} while ((c = getchr()) != '\n');
	*p1++ = 0;
	if (savedfile[0]==0 || comm=='e' || comm=='f') {
		p1 = savedfile;
		p2 = file;
		while ((*p1++ = *p2++))
			;
	}
}

int getchr(void) {
	char c;
	if ((lastc=peekc)) {
		peekc = 0;
		return(lastc);
	}
	if (globp) {
		if ((lastc = *globp++) != 0)
			return(lastc);
		globp = 0;
		return(EOF);
	}
	if (read(0, &c, 1) <= 0)
		return(lastc = EOF);
	lastc = c&0177;
	return(lastc);
}

int getfile(void) {
	int c;
	char *lp, *fp;

	lp = linebuf;
	fp = nextip;
	do {
		if (--ninbuf < 0) {
			if ((ninbuf = read(io, genbuf, LBSIZE)-1) < 0){
				if (lp>linebuf) {
					puts("newline appended", 0);
					*genbuf = '\n';
				}
				else {return(EOF);}
			}
			fp = genbuf;
			while(fp < &genbuf[ninbuf]) {
				if (*fp++ & 0200)
					break;
			}
			fp = genbuf;
		}
		c = *fp++;
		if (c=='\0')
			continue;
		// if (c&0200 || lp >= &linebuf[LBSIZE]) {
		// 	lastc = '\n';
		// }
		*lp++ = c;
		count++;
	} while (c != '\n');
	*--lp = 0;
	nextip = fp;
	return(0);
}

//need
int append(int (*f)(void), unsigned int *a) {
	unsigned int *a1, *a2, *rdot;
	int nline, tl;

	nline = 0;
	dot = a;
	while ((*f)() == 0) {
		tl = putline();
		nline++;
		a1 = ++dol;
		a2 = a1+1;
		rdot = ++dot;
		while (a1 > rdot)
			*--a2 = *--a1;
		*rdot = tl;
	}
	return(nline);
}

char *getline(unsigned int tl) {
	char *bp, *lp;
	int nl;

	lp = linebuf;
	bp = getblock(tl, READ);
	tl &= ~((BLKSIZE/2)-1);
	while ((*lp++ = *bp++))
		if (--nl == 0) {
			bp = getblock(tl+=(BLKSIZE/2), READ);
		}
	return(linebuf);
}

int putline(void) {
	char *bp, *lp;
	int nl;
	unsigned int tl;

	lp = linebuf;
	tl = tline;
	bp = getblock(tl, WRITE);
	tl &= ~((BLKSIZE/2)-1);
	while ((*bp = *lp++)) {
		if (*bp++ == '\n') {
			*--bp = 0;
			linebp = lp;
			break;
		}
	}
	nl = tline;
	tline += (((lp-linebuf)+03)>>1)&077776;
	return(nl);
}

char *getblock(unsigned int atl, int iof) {
	int bno, off;

	bno = (atl/(BLKSIZE/2));
	off = (atl<<1) & (BLKSIZE-1) & ~03;
	if (bno==oblock)
		return(obuff+off);
	return(obuff+off);
}

void init(void) {
	dot = dol = zero;
}

//IMPORTANT!!!!
void global(int k) {
	char *gp;
	int c;
	unsigned int *a1;
	char globuf[GBSIZE];

	setwide();
	if ((c=getchr())=='\n'){}
	compile(c);
	gp = globuf;
	while ((c = getchr()) != '\n') {

		if (c=='\\') {
			c = getchr();
			if (c!='\n')
				*gp++ = '\\';
		}
		*gp++ = c;
	}
	if (gp == globuf)
		*gp++ = 'p';
	*gp++ = '\n';
	*gp++ = 0;
	for (a1=zero; a1<=dol; a1++) {
		*a1 &= ~01;
		if (a1>=addr1 && a1<=addr2 && execute(a1)==k)
			*a1 |= 01;
	}
	for (a1=zero; a1<=dol; a1++) {
		if (*a1 & 01) {
			*a1 &= ~01;
			dot = a1;
			globp = globuf;
			commands();
			a1 = zero;
		}
	}
}

//IMPORTANT!!!!!!!!!!
void compile(int eof) {
	int c;
	char *ep;
	char *lastep;
	char bracket[NBRA], *bracketp;
	int cclcnt;

	ep = expbuf;
	bracketp = bracket;
	if ((c = getchr()) == '\n') {
		peekc = c;
		c = eof;
	}
	if (c == eof) {
		return;
	}
	nbra = 0;
	if (c=='^') {
		c = getchr();
		*ep++ = CCIRC;
	}
	peekc = c;
	lastep = 0;
	for (;;) {
		c = getchr();
		if (c == '\n') {
			peekc = c;
			c = eof;
		}
		if (c==eof) {
			*ep++ = CEOF;
			return;
		}
		if (c!='*')
			lastep = ep;
		switch (c) {
		//need for curly brace implmentation to escape brace char
		case '\\':
			if ((c = getchr())=='(') {
				if (nbra >= NBRA){
					expbuf[0] = 0;
					nbra = 0;
					return;
					//goto cerror;
				}
				*bracketp++ = nbra;
				*ep++ = CBRA;
				*ep++ = nbra++;
				continue;
			}
			if (c == ')') {
				if (bracketp <= bracket){
					expbuf[0] = 0;
					nbra = 0;
					return;
					//goto cerror;
				}
				*ep++ = CKET;
				*ep++ = *--bracketp;
				continue;
			}
			if (c>='1' && c<'1'+NBRA) {
				*ep++ = CBACK;
				*ep++ = c-'1';
				continue;
			}
			*ep++ = CCHR;
			if (c=='\n'){
				expbuf[0] = 0;
				nbra = 0;
				return;
				//goto cerror;
			}
			*ep++ = c;
			continue;

		case '*':
			if (lastep==0 || *lastep==CBRA || *lastep==CKET){
				*ep++ = CCHR;
				*ep++ = c;
				continue;
				//goto defchar;
			}
			*lastep |= STAR;
			continue;
		//end of line $  search
		case '$':
			if ((peekc=getchr()) != eof && peekc!='\n'){
				*ep++ = CCHR;
				*ep++ = c;
				continue;
				//goto defchar;
			}
			*ep++ = CDOL;
			continue;
		//for [0-9] and [A-Z] searches
		case '[':
			*ep++ = CCL;
			*ep++ = 0;
			cclcnt = 1;
			if ((c=getchr()) == '^') {
				c = getchr();
				ep[-2] = NCCL;
			}
			do {
				if (c=='\n'){
					expbuf[0] = 0;
					nbra = 0;
					return;
					//goto cerror;
				}
				if (c=='-' && ep[-1]!=0) {
					if ((c=getchr())==']') {
						*ep++ = '-';
						cclcnt++;
						break;
					}
					while (ep[-1]<c) {
						*ep = ep[-1]+1;
						ep++;
						cclcnt++;
						if (ep>=&expbuf[ESIZE]){
							expbuf[0] = 0;
							nbra = 0;
							return;
							//goto cerror;
						}
					}
				}
				*ep++ = c;
				cclcnt++;
				if (ep >= &expbuf[ESIZE]){
					expbuf[0] = 0;
					nbra = 0;
					return;
				}
			} while ((c = getchr()) != ']');
			lastep[1] = cclcnt;
			continue;

		default:
			*ep++ = CCHR;
			*ep++ = c;
		}
	}
}

//need
int execute(unsigned int *addr) {
	char *p1, *p2;
	int c;

	p2 = expbuf;
	p1 = getline(*addr);
	if (*p2==CCIRC) {
		return(advance(p1, p2+1));
	}
	//need
	/* regular algorithm */
	do {
		if (advance(p1, p2)) {
			return(1);
		}
	} while (*p1++);
	return(0);
}

//need
int advance(char *lp, char *ep) {
	for (;;) switch (*ep++) {
	//need!
	case CCHR:
		if (*ep++ == *lp++)
			continue;
		return(0);
	//need
	case CDOL:
		if (*lp==0)
			continue;
		return(0);
  //need!
	case CEOF:
		return(1);
	//need
	case CCL:
		if (cclass(ep, *lp++, 1)) {
			ep += *ep;
			continue;
		}
		return(0);
	}
}

//need
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

//need
void puts(char *sp, int fname) {
	while (*sp)
		putchr(*sp++);
	if(!fname){
		putchr('\n');
	}
	else{
		putchr(':');
		putchr(' ');
	}
}

char	line[70];
char	*linp	= line;
//need
void putchr(int ac) {
	char *lp;
	int c;

	lp = linp;
	c = ac;

	*lp++ = c;
	if(c == '\n' || lp >= &line[64]) {
		linp = line;
		write(oflag?2:1, line, lp-line);
		return;
	}
	linp = lp;
}
