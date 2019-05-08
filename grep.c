#include "grep.h"

#define	BLKSIZE	4096
#define	FNSIZE	128
#define	LBSIZE	4096
#define	ESIZE	256
#define	GBSIZE	256
#define	NBRA	5
#define	EOF	-1
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

static char	line[70],	savedfile[FNSIZE], file[FNSIZE], linebuf[LBSIZE],	expbuf[ESIZE+4], genbuf[LBSIZE], obuff[BLKSIZE], *braslist[NBRA], *braelist[NBRA];
static char	*linp	= line,	*nextip, *globp, *loc2, *files[1000], *pattern, *fname;
static int	tline, p_length = 0, p_index = 0, mfiles = 0, numfiles = 0,	peekc, lastc,	given,	ninbuf,	io,	oflag, oblock	= -1, nbra;

static long	count;
static unsigned int	*addr1, *addr2, *dot, *dol, *zero;
static unsigned nlall = 128;

typedef struct dirent dirent;
static DIR* dir;
static dirent *in_file;


int main(int argc, char *argv[]) {
	char *p1, *p2;
	if(argc <= 2){
		puts("Error: Too little arguments\nUsage: grep SEARCH_PATTERN [FILES...]", 0);
		return 0;
	}
  //create regular expression prefied by g/ to search globally in the  file and postfixed bp /p signal print
	argv++;
	int j;
	pattern = (char *) malloc(sizeof(char) * (strlen(*argv) + 5));
	pattern[0] = 'g';
	pattern[1] = '/';
	for(j = 0; j < (int) strlen(*argv); j++){	pattern[j + 2] = (*argv)[j]; }
	pattern[j + 2] = '/';
	pattern[j + 3] = 'p';
	pattern[j + 4] = '\0';
	p_length = (int) strlen(pattern);
	//set up possible directory
	argv++;
	fname = *argv;
	dir = opendir(fname);
	//if directory: (grep hello /documents/grep )
	if(dir == NULL){ closedir(dir);	}
	else{
		while((in_file = readdir(dir)) != NULL){ files[numfiles++] = in_file->d_name;	}
		if(numfiles > 0){	mfiles = 1; }
	}
	//if listed multiple files: (grep hello hello.txt yes.txt ...)
	if(argc > 3){
		mfiles = 1;
		//put all filenames into file[]
		for(int x = 2; x < argc; x++){ files[numfiles++] = *(argv + x - 2);	}
	}
	//if single file: (grep hello hello.txt)
	if(numfiles == 0){
		files[0] = *argv;
		numfiles = 1;
	}
	//loop through all files
	for(int i = 0; i < numfiles; i++){
		p_index = 0;
		p1 = files[i];
		fname = p1;
		p2 = savedfile;
		while ((*p2++ = *p1++)){
			if (p2 >= &savedfile[sizeof(savedfile)]){	p2--;	}
		}
		globp = "e"; // changed to "e" in order to signal in commands() to open file
		zero = (unsigned *)malloc(nlall*sizeof(unsigned));//need
		dot = dol = zero;
		commands();
	}
	return 0;
}

unsigned int *address(void) {
	int sign = 1, opcnt = 0, nextopand = -1, c;
	unsigned int *a = dot;
	sign = 1;
	do {
		do c = getchr(); while (c==' ' || c=='\t');
		switch (c) {
		default:
			if (nextopand == opcnt) {
				a += sign;
				if (a<zero || dol<a)	continue;
			}
			if (c!='+' && c!='-' && c!='^') {
				peekc = c;
				if (opcnt==0)	a = 0;
				return (a);
			}
			sign = 1;
			if (c!='+')	sign = -sign;
			nextopand = ++opcnt;
			continue;
		}
	} while (zero<=a && a<=dol);
	return 0;
}
int advance(char *lp, char *ep) {
  for (;;) {
    switch (*ep++) {
      case CCHR:  if (*ep++ == *lp++) { continue; } return(0);
      case CDOT:  if (*lp++) { continue; }    return(0);
      case CDOL:  if (*lp==0) { continue; }  return(0);
      case CEOF:  loc2 = lp;  return(1);
      case CCL:   if (cclass(ep, *lp++, 1)) {  ep += *ep;  continue; }  return(0);
      case NCCL:  if (cclass(ep, *lp++, 0)) { ep += *ep;  continue; }  return(0);
      case CBRA:  braslist[(int)*ep++] = lp;  continue;
      case CKET:  braelist[(int)*ep++] = lp;  continue;
    }
  }
}
int append(int (*f)(void), unsigned int *a) {
	unsigned int *a1, *a2, *rdot;
	int nline, tl;
	nline = 0;  dot = a;
  while ((*f)() == 0) {
    if ((dol-zero)+1 >= nlall) {
			unsigned *ozero = zero;
			nlall += 1024;
      if ((zero = (unsigned *)realloc((char *)zero, nlall*sizeof(unsigned)))==NULL) { }
      dot += zero - ozero;
			dol += zero - ozero;
    }
    tl = putline();
		nline++;
		a1 = ++dol; a2 = a1+1; rdot = ++dot;
    while (a1 > rdot) { *--a2 = *--a1; }
		*rdot = (unsigned) tl;
  }
  return(nline);
}
int cclass(char *set, int c, int af) {
	int n = *set++;
	if (c==0){ return(0); }
	while (--n)
		if (*set++ == c)
			return(af);
	return(!af);
}
void commands(void) {
	unsigned int *a1;
	int c;
	char lastsep;
	for (;;) {
	c = '\n';
	for (addr1 = 0;;) {
		lastsep = (char) c;	a1 = address();	c = getchr();
		if (c!=',' && c!=';')	break;
		if (a1==0) {
			a1 = zero+1;
			if (a1>dol)	a1--;
		}
		addr1 = a1;
		if (c==';')	dot = a1;
	}
	if ((addr2=a1)==0) {given = 0;addr2 = dot;}
	else	given = 1;
	if (addr1==0)	addr1 = addr2;
	switch(c) {
		case 'g':
			global(1);	continue;
		case 'p':	case 'P':
			newline();	print();	continue;
		case 'e':
			filename(c);
			if ((io = open(savedfile, 0)) < 0){
					puts("Error opening file: ",0);
					puts(savedfile, 0);
					return;
			}
			setwide();
			ninbuf = 0;
			append(getfile, addr2);
			close(io);
			io = -1;
			continue;
		case EOF:	return;
		}
	}
}
void compile(int eof) {
	int c, cclcnt;
	char *ep = expbuf, *lastep, bracket[NBRA], *bracketp = bracket;
  if ((c = getchr()) == '\n') { peekc = c;  c = eof; }
  if (c == eof) {  return; }
  nbra = 0;  if (c=='^') { c = getchr();  *ep++ = CCIRC; }  peekc = c;  lastep = 0;
  for (;;) {
    if (ep >= &expbuf[ESIZE]) { expbuf[0] = 0;  nbra = 0;  }  c = getchr();
		if (c == '\n') { peekc = c;  c = eof; }
    if (c==eof) { if (bracketp != bracket) { expbuf[0] = 0;  nbra = 0;  }  *ep++ = CEOF;  return;  }
    if (c!='*') { lastep = ep; }
    switch (c) {
      case '\\':
        if ((c = getchr())=='(') {
          if (nbra >= NBRA) { expbuf[0] = 0;  nbra = 0; }  *bracketp++ = (char) nbra;  *ep++ = CBRA;  *ep++ = (char) nbra++;  continue;
        }
        if (c == ')') {  if (bracketp <= bracket) { expbuf[0] = 0;  nbra = 0;  }  *ep++ = CKET;  *ep++ = *--bracketp;  continue; }
        if (c>='1' && c<'1'+NBRA) { *ep++ = CBACK;  *ep++ = (char) c-'1';  continue; }
        *ep++ = CCHR;  if (c=='\n') { expbuf[0] = 0;  nbra = 0; }  *ep++ = (char) c;  continue;
      case '.': *ep++ = CDOT;  continue;
      case '\n':  expbuf[0] = 0;  nbra = 0;
      case '*':  if (lastep==0 || *lastep==CBRA || *lastep==CKET) { *ep++ = CCHR;  *ep++ = (char) c; }
				*lastep |= STAR; continue;
      case '$':  if ((peekc=getchr()) != eof && peekc!='\n') { *ep++ = CCHR;  *ep++ = (char) c; }  *ep++ = CDOL;  continue;
      case '[':  *ep++ = CCL;  *ep++ = 0;  cclcnt = 1;
				if ((c=getchr()) == '^') {  c = getchr();
					 ep[-2] = NCCL; }
        do {
          if (c=='\n') { expbuf[0] = 0;  nbra = 0; }  if (c=='-' && ep[-1]!=0) {
            if ((c=getchr())==']') { *ep++ = '-';
						  cclcnt++;  break; }
            while (ep[-1] < c) {  *ep = ep[-1] + 1;
						  ep++;
							cclcnt++;
							if (ep >= &expbuf[ESIZE]) { expbuf[0] = 0;
								nbra = 0;}
							}
          }
          *ep++ = (char) c;  cclcnt++;
					if (ep >= &expbuf[ESIZE]) { expbuf[0] = 0;
						  nbra = 0; }
        } while ((c = getchr()) != ']');
        lastep[1] = (char) cclcnt;  continue;
			default: *ep++ = CCHR;  *ep++ = (char) c;
    }
  }
}
int execute(unsigned int *addr) {
	char *p1, *p2;
	p2 = expbuf;
	p1 = getline(*addr);
	if (*p2==CCIRC) {	return(advance(p1, p2+1)); }
	do {
		if (advance(p1, p2)) {	return(1);	}
	} while (*p1++);
	return(0);
}
void filename(int comm) {
	char *p1, *p2;  int c = getchr();  count = 0;
	if (c == '\n' || c == EOF) {
    p1 = savedfile;
    p2 = file;
    while ((*p2++ = *p1++)) { }
    return;
  }
  while ((c = getchr()) == ' ') { }   p1 = file;
  do {  *p1++ = (char) c;  } while ((c = getchr()) != '\n');
  *p1++ = 0;
  if (savedfile[0] == 0||comm == 'e'||comm == 'f') {
		p1 = savedfile;
		p2 = file;
    while ((*p1++ = *p2++)) { }
  }
}
char *getblock(unsigned int atl) {
	int bno, off;
	bno = (atl/(BLKSIZE/2));
	off = (int) (atl<<1) & (BLKSIZE-1) & ~03;
	if (bno==oblock) return(obuff+off);
	return(obuff+off);
}
int getchr(void) {
	if ((lastc=peekc)) { peekc = 0; return(lastc); }
	if (globp) {
		if ((lastc = *globp++) != 0) {return(lastc);}
		globp = 0;
		return(EOF);
	}
	if (p_index == p_length) {return(lastc = EOF);}
	lastc = pattern[p_index++];
	return(lastc);
}
int getfile(void) {
	int c;
	char *lp = linebuf, *fp = nextip;
	do {
		if (--ninbuf < 0) {
			if ((ninbuf = read(io, genbuf, LBSIZE)-1) < 0){
				if (lp>linebuf) {	*genbuf = '\n';	}
				else {return(EOF);}
			}
			fp = genbuf;
			while(fp < &genbuf[ninbuf]) {	if (*fp++ & 0200)	break;	}
			fp = genbuf;
		}
		c = *fp++;
		if (c=='\0') continue;
		if (c&0200 || lp >= &linebuf[LBSIZE]) {
			lastc = '\n';
		}
		*lp++ = (char) c;
		count++;
	} while (c != '\n');
	*--lp = 0;
	nextip = fp;
	return(0);
}
char *getline(unsigned int tl) {
	char *bp = getblock(tl), *lp = linebuf;
	int nl = 0;
	tl &= (unsigned) ~((BLKSIZE/2)-1);
	while ((*lp++ = *bp++))
		if (--nl == 0) { bp = getblock(tl+=(BLKSIZE/2)); }
	return(linebuf);
}
void global(int k) {
	char *gp, globuf[GBSIZE];
	int c;
	unsigned int *a1;
	setwide();
	if ((c=getchr())=='\n') {}
	compile(c);
	gp = globuf;
	while ((c = getchr()) != '\n' && p_index < p_length) {
		if (c=='\\') { c = getchr(); if (c!='\n') {*gp++ = '\\';} }
		*gp++ = (char) c;
	}
	if (gp == globuf) {*gp++ = 'p';}
	*gp++ = '\n';
	*gp++ = 0;
	for (a1=zero; a1<=dol; a1++) {
		*a1 &= (unsigned) ~01;
		if (a1>=addr1 && a1<=addr2 && execute(a1)==k) {*a1 |= 01;}
	}
	for (a1=zero; a1<=dol; a1++) {
		if (*a1 & 01) {
			*a1 &= (unsigned) ~01;
			dot = a1;
			globp = globuf;
			commands();
			a1 = zero;
		}
	}
}
void newline(void) {
	int c;
	if ((c = getchr()) == '\n' || c == EOF)	return;
	if (c=='p' || c=='l' || c=='n') {
		if ((c=getchr())=='\n')	return;
	}
}
void print(void) {
	unsigned int *a1 = addr1;
	do {
		if(mfiles){	puts(fname, mfiles);}
		puts(getline(*a1++),0);
	} while (a1 <= addr2);
	dot = addr2;
}
int putline(void) {
	unsigned int tl = (unsigned) tline;
	char *bp = getblock(tl), *lp = linebuf;
	int nl;
	tl &= (unsigned) ~((BLKSIZE/2)-1);
	while ((*bp = *lp++)) {	if (*bp++ == '\n') {	*--bp = 0;	break;	}}
	nl = tline;
	tline += (((lp-linebuf)+03)>>1)&077776;
	return(nl);
}
void puts(char *sp, int mflag) {
	while (*sp) 	putchr(*sp++);
	if(!mflag){		putchr('\n');	}
	else{
		putchr(':');
		putchr(' ');
	}
}
void putchr(int ac) {
	char *lp;
	int c = ac;
	lp = linp;
	*lp++ = (char) c;
	if(c == '\n' || lp >= &line[64]) {
		linp = line;
		write(oflag?2:1, line, (int)(lp-line));
		return;
	}
	linp = lp;
}
void setwide(void) { if (!given) {	addr1 = zero + (dol>zero); addr2 = dol;	} }
