#ifndef grep_h
#define grep_h

#include <string.h>
#include <stdlib.h>
#include <dirent.h>

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
#define	READ	0
#define	WRITE	1

char	Q[]	= "", T[]	= "TMP", line[70],	savedfile[FNSIZE], file[FNSIZE], linebuf[LBSIZE],	expbuf[ESIZE+4], genbuf[LBSIZE], obuff[BLKSIZE], *braslist[NBRA], *braelist[NBRA];
char	*linp	= line,	*nextip, *globp, *tfname, *loc1, *loc2, *files[1000], *pattern, *fname;
int	tline, p_length = 0, p_index = 0, mfiles = 0, numfiles = 0,	peekc, lastc,	given,	ninbuf,	io,	oflag, col, oblock	= -1, nbra, names[26], tfile	= -1;

long	count;
unsigned int	*addr1, *addr2, *dot, *dol, *zero;
unsigned nlall = 128;

long	lseek(int, long, int);
int	open(char *, int);
int	creat(char *, int);
int	read(int, char*, int);
int	write(int, char*, int);
int	close(int);
int	fork(void);
int	execl(char *, ...);
int	wait(int *);
int	unlink(char *);

int backref(int i, char *lp);
char *getblock(unsigned int atl, int iof);
char *getline(unsigned int tl);
int advance(char *lp, char *ep);
int append(int (*f)(void), unsigned int *a);
int cclass(char *set, int c, int af);
void commands(void);
void compile(int eof);
void error(char *s);
int execute(unsigned int *addr);
void exfile(void);
void filename(int comm);
int close(int);
int getchr(void);
int getfile(void);
void global(int k);
unsigned int *address(void);
void newline(void);
void print(void);
void putchr(int ac);
void putd(void);
int putline(void);
void puts(char *sp, int mfiles);
void setwide(void);

typedef struct dirent dirent;
DIR* dir;
dirent *in_file;
#endif
