#ifndef grep_h
#define grep_h

/* make BLKSIZE and LBSIZE 512 for smaller machines */
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

#define	BLKSIZE	4096
#define	NBLK	2047
#define	FNSIZE	128
#define	LBSIZE	4096
#define	ESIZE	256
#define	GBSIZE	256
#define	NBRA	5
#define	EOF	-1
#define	KSIZE	9

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
char	T[]	= "TMP";
#define	READ	0
#define	WRITE	1

char	line[70];
char	*linp	= line;
int	peekc;
int	lastc;
char	savedfile[FNSIZE];
char	file[FNSIZE];
char	linebuf[LBSIZE];
char	expbuf[ESIZE+4];
int	given;
unsigned int	*addr1, *addr2;
unsigned int	*dot, *dol, *zero;
char	genbuf[LBSIZE];
long	count;
char	*nextip;
int	ninbuf;
int	io;
int	pflag;
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

int	oflag;
int	listn;
int	col;
char	*globp;
int	tline;
char	*tfname;
char	*loc1;
char	*loc2;
char	obuff[BLKSIZE];
int	oblock	= -1;
int	names[26];
char	*braslist[NBRA];
char	*braelist[NBRA];
int	nbra;
unsigned nlall = 128;
int	tfile	= -1;

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

char* files[1000];
char* pattern;
char* fname;
int p_length = 0;
int p_index = 0;
int mfiles = 0;
int numfiles = 0;

#endif
