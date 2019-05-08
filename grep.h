#ifndef grep_h
#define grep_h

#include <string.h>
#include <stdlib.h>
#include <dirent.h>

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

char *getblock(unsigned int atl);
char *getline(unsigned int tl);
int advance(char *lp, char *ep);
int append(int (*f)(void), unsigned int *a);
int cclass(char *set, int c, int af);
void commands(void);
void compile(int eof);
int execute(unsigned int *addr);
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

#endif
