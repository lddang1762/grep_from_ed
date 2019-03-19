#ifndef grep_h
#define grep_h

/* make BLKSIZE and LBSIZE 512 for smaller machines */

#define	BLKSIZE	4096

#define	NULL	0
#define	FNSIZE	128
#define	LBSIZE	4096
#define	ESIZE	256
#define	GBSIZE	256
#define	NBRA	5
#define	EOF	-1

#define	CBRA	1
#define	CCHR	2
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
char	*linebp;
int	ninbuf;
int	io;
int	pflag;
int	open(char *, int);
int	read(int, char*, int);
int	write(int, char*, int);

int	vflag	= 1;
int	oflag;
int	listf;
int	listn;
char	*globp;
int	tfile	= -1;
int	tline;
char	*loc2;
char	obuff[BLKSIZE];
int	oblock	= -1;
int	nbra;
unsigned nlall = 128;

char	*malloc(int);

char *getblock(unsigned int atl, int iof);
char *getline(unsigned int tl);
int advance(char *lp, char *ep);
int append(int (*f)(void), unsigned int *a);
void blkio(int b, char *buf, int (*iofcn)(int, char*, int));
int cclass(char *set, int c, int af);
void commands(void);
void compile(int eof);//MOST IMPORTANT!!!
void error(char *s);
int execute(unsigned int *addr);
void exfile(void);
void filename(int comm);
int getchr(void);
int getfile(void);
void global(int k);
void init(void);
unsigned int *address(void);
void newline(void);
void print(void);
void putchr(int ac);
void putd(void);
int putline(void);
void puts(char *sp);
void setwide(void);

jmp_buf	savej;

#define MAXSIZE 500
char *pattern;

#endif
