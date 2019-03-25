#include "grep.h"

int main(int argc, char *argv[]) {
	char *p1, *p2;
	if(argc <= 2){
		puts("Error: Too little arguments\nUsage: grep SEARCH_PATTERN [FILES...]", 0);
		return 0;
	}

  //create regular expression prefied by g/ to search globally in the  file and postfixed bp /p signal print
	argv++;
	int i;
	pattern = (char *) malloc(sizeof(char) * (strlen(*argv) + 5));
	pattern[0] = 'g';
	pattern[1] = '/';
	for(i = 0; i < strlen(*argv); i++){
		pattern[i + 2] = (*argv)[i];
	}
	pattern[i + 2] = '/';
	pattern[i + 3] = 'p';
	pattern[i + 4] = '\0';

	p_length = strlen(pattern);
	
	//set up possible directory
	argv++;
	fname = *argv;
	dir = opendir(fname);

	//if directory: (grep hello /documents/grep )
	if(dir == NULL){
		closedir(dir);
	}
	else{
		while((in_file = readdir(dir)) != NULL){
			files[numfiles++] = in_file->d_name;
		}
		if(numfiles > 0){
			mfiles = 1;
			//puts("multiple files!\n", 0);
		}
	}

	//if listed multiple files: (grep hello hello.txt yes.txt ...)
	if(argc > 3){
		mfiles = 1;
		//put all filenames into file[]
		for(int x = 2; x < argc; x++){
			files[numfiles++] = *(argv + x - 2);
		}
		//puts("Multiple files!\n", 0);
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
			if (p2 >= &savedfile[sizeof(savedfile)]){
				p2--;
			}
		}
		globp = "e"; // changed to "e" in order to signal in commands() to open file
		zero = (unsigned *)malloc(nlall*sizeof(unsigned));//need
		dot = dol = zero;
		commands();
	}
	return 0;
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
int advance(char *lp, char *ep) {  char *curlp;  int i;
  for (;;) {
    switch (*ep++) {
      case CCHR:  if (*ep++ == *lp++) { continue; } return(0);
      case CDOT:  if (*lp++) { continue; }    return(0);
      case CDOL:  if (*lp==0) { continue; }  return(0);
      case CEOF:  loc2 = lp;  return(1);
      case CCL:   if (cclass(ep, *lp++, 1)) {  ep += *ep;  continue; }  return(0);
      case NCCL:  if (cclass(ep, *lp++, 0)) { ep += *ep;  continue; }  return(0);
      case CBRA:  braslist[*ep++] = lp;  continue;
      case CKET:  braelist[*ep++] = lp;  continue;
      case CBACK:
        if (braelist[i = *ep++] == 0) { error(Q); }
        if (backref(i, lp)) { lp += braelist[i] - braslist[i];  continue; }  return(0);
      case CBACK|STAR:
        if (braelist[i = *ep++] == 0) { error(Q); }  curlp = lp;
        while (backref(i, lp)) { lp += braelist[i] - braslist[i]; }
        while (lp >= curlp) {  if (advance(lp, ep)) { return(1); }  lp -= braelist[i] - braslist[i];  }  continue;
      case CDOT|STAR:  curlp = lp;  while (*lp++) { }
				do {  lp--;  if (advance(lp, ep)) { return(1); } } while (lp > curlp);
				return(0);
      case CCHR|STAR:  curlp = lp;  while (*lp++ == *ep) { }  ++ep;
				do {  lp--;  if (advance(lp, ep)) { return(1); } } while (lp > curlp);
				return(0);
      case CCL|STAR:
      case NCCL|STAR:  curlp = lp;  while (cclass(ep, *lp++, ep[-1] == (CCL|STAR))) { }  ep += *ep;
				do {lp--;  if (advance(lp, ep)) {return(1);}}while (lp > curlp);
				return(0);
      default: error(Q);
    }
  }
}
int append(int (*f)(void), unsigned int *a) {  unsigned int *a1, *a2, *rdot;  int nline, tl;  nline = 0;  dot = a;
  while ((*f)() == 0) {
    if ((dol-zero)+1 >= nlall) {  unsigned *ozero = zero;  nlall += 1024;
      if ((zero = (unsigned *)realloc((char *)zero, nlall*sizeof(unsigned)))==NULL) {  error("MEM?");   }
      dot += zero - ozero;  dol += zero - ozero;
    }
    tl = putline();  nline++;  a1 = ++dol;  a2 = a1+1;  rdot = ++dot;
    while (a1 > rdot) { *--a2 = *--a1; }  *rdot = tl;
  }
  return(nline);
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
void commands(void) {
	unsigned int *a1;	int c;	int temp;	char lastsep;

	for (;;) {
	c = '\n';
	for (addr1 = 0;;) {
		lastsep = c;	a1 = address();	c = getchr();
		if (c!=',' && c!=';')	break;
		if (a1==0) {	a1 = zero+1;	if (a1>dol)	a1--;	}
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
			exfile();
			continue;

		case EOF:	return;
		}
		error(Q);
	}
}
void compile(int eof) {  int c, cclcnt;  char *ep = expbuf, *lastep, bracket[NBRA], *bracketp = bracket;
  if ((c = getchr()) == '\n') { peekc = c;  c = eof; }
  if (c == eof) {  if (*ep==0) { error(Q); }  return; }
  nbra = 0;  if (c=='^') { c = getchr();  *ep++ = CCIRC; }  peekc = c;  lastep = 0;
  for (;;) {
    if (ep >= &expbuf[ESIZE]) { expbuf[0] = 0;  nbra = 0;  error(Q);; }  c = getchr();  if (c == '\n') { peekc = c;  c = eof; }
    if (c==eof) { if (bracketp != bracket) { expbuf[0] = 0;  nbra = 0;  error(Q);; }  *ep++ = CEOF;  return;  }
    if (c!='*') { lastep = ep; }
    switch (c) {
      case '\\':
        if ((c = getchr())=='(') {
          if (nbra >= NBRA) { expbuf[0] = 0;  nbra = 0;  error(Q);; }  *bracketp++ = nbra;  *ep++ = CBRA;  *ep++ = nbra++;  continue;
        }
        if (c == ')') {  if (bracketp <= bracket) { expbuf[0] = 0;  nbra = 0;  error(Q);; }  *ep++ = CKET;  *ep++ = *--bracketp;  continue; }
        if (c>='1' && c<'1'+NBRA) { *ep++ = CBACK;  *ep++ = c-'1';  continue; }
        *ep++ = CCHR;  if (c=='\n') { expbuf[0] = 0;  nbra = 0;  error(Q);; }  *ep++ = c;  continue;
      case '.': *ep++ = CDOT;  continue;
      case '\n':  expbuf[0] = 0;  nbra = 0;  error(Q);;
      case '*':  if (lastep==0 || *lastep==CBRA || *lastep==CKET) { *ep++ = CCHR;  *ep++ = c;; }  *lastep |= STAR; continue;
      case '$':  if ((peekc=getchr()) != eof && peekc!='\n') { *ep++ = CCHR;  *ep++ = c;; }  *ep++ = CDOL;  continue;
      case '[':  *ep++ = CCL;  *ep++ = 0;  cclcnt = 1;  if ((c=getchr()) == '^') {  c = getchr();  ep[-2] = NCCL; }
        do {
          if (c=='\n') { expbuf[0] = 0;  nbra = 0;  error(Q);; }  if (c=='-' && ep[-1]!=0) {
            if ((c=getchr())==']') { *ep++ = '-';  cclcnt++;  break; }
            while (ep[-1] < c) {  *ep = ep[-1] + 1;  ep++;  cclcnt++;  if (ep >= &expbuf[ESIZE]) { expbuf[0] = 0;  nbra = 0;  error(Q);; } }
          }
          *ep++ = c;  cclcnt++;  if (ep >= &expbuf[ESIZE]) { expbuf[0] = 0;  nbra = 0;  error(Q);; }
        } while ((c = getchr()) != ']');
        lastep[1] = cclcnt;  continue;
			default: *ep++ = CCHR;  *ep++ = c;
    }
  }
}
void error(char *s) {
	int c;  listn = 0;  putchr('?');  puts(s,0);
  count = 0;  lseek(0, (long)0, 2);  pflag = 0;
	if (globp) { lastc = '\n'; }  globp = 0;  peekc = lastc;
  if(lastc) { while ((c = getchr()) != '\n' && c != EOF) { } }
  if (io > 0) { close(io);  io = -1; }
}
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
void exfile(void) {  close(io);  io = -1; }
void filename(int comm) {
	char *p1, *p2;  int c;  count = 0;  c = getchr();
	if (c == '\n' || c == EOF) {
    p1 = savedfile;
    if (*p1 == 0 && comm != 'f') { error(Q); }
    p2 = file;
    while ((*p2++ = *p1++)) { }
    return;
  }
  if (c!=' ') { error(Q); }
  while ((c = getchr()) == ' ') { }  if (c=='\n') { error(Q); }  p1 = file;
  do {
    if (p1 >= &file[sizeof(file) - 1] || c == ' ' || c == EOF) { error(Q); }  *p1++ = c;
  } while ((c = getchr()) != '\n');
  *p1++ = 0;
  if (savedfile[0] == 0||comm == 'e'||comm == 'f') { p1 = savedfile;  p2 = file;
    while ((*p1++ = *p2++)) { }
  }

}
char *getblock(unsigned int atl, int iof) {
	int bno, off;

	bno = (atl/(BLKSIZE/2));
	off = (atl<<1) & (BLKSIZE-1) & ~03;
	if (bno==oblock)
		return(obuff+off);
	return(obuff+off);
}
int getchr(void) {
	char c;
	if ((lastc=peekc)) { peekc = 0; return(lastc); }
	if (globp) { if ((lastc = *globp++) != 0) {return(lastc);} globp = 0; return(EOF); }
	if (p_index == p_length) {return(lastc = EOF);}
	lastc = pattern[p_index]; p_index++;
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
				//	puts("newline appended", 0);
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
		if (c&0200 || lp >= &linebuf[LBSIZE]) {
			lastc = '\n';
			error(Q);
		}
		*lp++ = c;
		count++;
	} while (c != '\n');
	*--lp = 0;
	nextip = fp;
	return(0);
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
void global(int k) {
	char *gp; int c; unsigned int *a1; char globuf[GBSIZE];
	if (globp) {error(Q);}
	setwide();
	if ((c=getchr())=='\n') {error(Q);}
	compile(c);
	gp = globuf;
	while ((c = getchr()) != '\n' && p_index < p_length) {
		if (c==EOF) {error(Q);}
		if (c=='\\') { c = getchr(); if (c!='\n') {*gp++ = '\\';} }
		*gp++ = c;
		if (gp >= &globuf[GBSIZE-2]) {error(Q);}
	}
	if (gp == globuf) {*gp++ = 'p';}
	*gp++ = '\n'; *gp++ = 0;
	for (a1=zero; a1<=dol; a1++) { *a1 &= ~01; if (a1>=addr1 && a1<=addr2 && execute(a1)==k) {*a1 |= 01;}}
	for (a1=zero; a1<=dol; a1++) { if (*a1 & 01) { *a1 &= ~01; dot = a1; globp = globuf; commands(); a1 = zero; } }
}
void newline(void) {
	int c;

	if ((c = getchr()) == '\n' || c == EOF)
		return;
	if (c=='p' || c=='l' || c=='n') {
		pflag++;
		if (c=='n')
			listn++;
		if ((c=getchr())=='\n')
			return;
	}
}
void print(void) {
	unsigned int *a1;

	a1 = addr1;
	do {
		if(mfiles){	puts(fname, mfiles);}
		puts(getline(*a1++),0);
	} while (a1 <= addr2);
	dot = addr2;
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
			break;
		}
	}
	nl = tline;
	tline += (((lp-linebuf)+03)>>1)&077776;
	return(nl);
}
void puts(char *sp, int mflag) {
	while (*sp)
		putchr(*sp++);
	if(!mflag){
		putchr('\n');
	}
	else{
		putchr(':');
		putchr(' ');
	}
}
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
void setwide(void) {
	if (!given) {
		addr1 = zero + (dol>zero);
		addr2 = dol;
	}
}
