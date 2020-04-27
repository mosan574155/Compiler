/*
 * token-list.h
 *
 *  Created on: 2018/10/26
 *      Author: centos
 */

/* token-list.h  */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXSTRSIZE 1024

/* Token */
#define	TNAME		1	/* Name : Alphabet { Alphabet | Digit } */
#define	TPROGRAM	2	/* program : Keyword */
#define	TVAR		3	/* var : Keyword */
#define	TARRAY		4	/* array : Keyword */
#define	TOF		5	/* of : Keyword */
#define	TBEGIN		6	/* begin : Keyword */
#define	TEND		7  	/* end : Keyword */
#define	TIF		8  	/* if : Keyword */
#define	TTHEN		9	/* then : Keyword */
#define	TELSE		10	/* else : Keyword */
#define	TPROCEDURE	11	/* procedure : Keyword */
#define	TRETURN		12	/* return : Keyword */
#define	TCALL		13	/* call : Keyword */
#define	TWHILE		14	/* while : Keyword */
#define	TDO		15 	/* do : Keyword */
#define	TNOT		16	/* not : Keyword */
#define	TOR		17	/* or : Keyword */
#define	TDIV		18 	/* div : Keyword */
#define	TAND		19 	/* and : Keyword */
#define	TCHAR		20	/* char : Keyword */
#define	TINTEGER	21	/* integer : Keyword */
#define	TBOOLEAN	22 	/* boolean : Keyword */
#define	TREADLN		23	/* readln : Keyword */
#define	TWRITELN	24	/* writeln : Keyword */
#define	TTRUE		25	/* true : Keyword */
#define	TFALSE		26	/* false : Keyword */
#define	TNUMBER		27	/* unsigned integer */
#define	TSTRING		28	/* String */
#define	TPLUS		29	/* + : symbol */
#define	TMINUS		30 	/* - : symbol */
#define	TSTAR		31 	/* * : symbol */
#define	TEQUAL		32 	/* = : symbol */
#define	TNOTEQ		33 	/* <> : symbol */
#define	TLE		34 	/* < : symbol */
#define	TLEEQ		35 	/* <= : symbol */
#define	TGR		36	/* > : symbol */
#define	TGREQ		37	/* >= : symbol */
#define	TLPAREN		38 	/* ( : symbol */
#define	TRPAREN		39 	/* ) : symbol */
#define	TLSQPAREN	40	/* [ : symbol */
#define	TRSQPAREN	41 	/* ] : symbol */
#define	TASSIGN		42	/* := : symbol */
#define	TDOT		43 	/* . : symbol */
#define	TCOMMA		44	/* , : symbol */
#define	TCOLON		45	/* : : symbol */
#define	TSEMI		46	/* ; : symbol */
#define	TREAD		47	/* read : Keyword */
#define	TWRITE		48	/* write : Keyword */
#define	TBREAK		49	/* break : Keyword */

#define NUMOFTOKEN	49

/* token-list.c */

#define KEYWORDSIZE	28

extern struct KEY {
	char * keyword;
	int keytoken;
} key[KEYWORDSIZE];

extern int error(char *mes);

/* scan.c */
extern int init_scan(char *filename);
extern int scan(void);
extern int num_attr;
extern char string_attr[MAXSTRSIZE];
extern int get_linenum(void);
extern void end_scan(void);

//addition//
FILE *fp;
int cbuf;
int linenum;
int num_attr;
char string_attr[MAXSTRSIZE];

extern void id_countup(char *np);
extern void print_idtab();
extern void release_idtab();
extern struct ID *search_idtab(char *np);
extern void output(char *tokenstr[NUMOFTOKEN + 1], int numtoken[NUMOFTOKEN + 1]);

//task2_addition//
//pretty//
int token;
extern int parse_program();
extern char *tokenstr[NUMOFTOKEN + 1];
#define NORMAL 0
#define ERROR 1

//task3_addition//
#define LOCAL 1
#define PROC 2
#define ARRAYA_SIZE_ID 20
struct ID {
	char name[ARRAYA_SIZE_ID];
	char procedure_name[ARRAYA_SIZE_ID];
	int ttype;
	int ispara;
	int arraysize;
	int arraytype;
	int defineline;
	int reflinenum[ARRAYA_SIZE_ID];
	struct ID *next;
	struct TYPE *type_next;
};

struct TYPE {
	int t_type;
	struct TYPE *t_next;
};

struct ID global, local;
struct ID *globalidroot, *localidroot;
int type_def;
int array_type_def;
int formal_flag;
int variabble_type;
int variabble_array_type;
int factor_type;
char tmp_procedure_name[ARRAYA_SIZE_ID];
int subpro_flag;
void check_scope(char *scope_keyword);
char *scope_keyword;
void add_newNode(char *name, char *procedure_name, int defineline);
void TypeRegister(struct ID *id, int type_def, int arraysize,
		int array_type_def);
void Set_various_Type(struct ID *id);
int RefRegister(struct ID *id);
int Dual_definision_check(struct ID *id);
void PrintIDTable_globalandlocal(struct ID *id);
void PrintIDTable(struct ID *id, struct ID *id2);

//task4_addition//
FILE *cslp;
int label_number;
char tmp_name[20];
char comp_procedure[20];
int writeflag;
extern void printlibrary();
extern void printstart();
extern void printdefine(struct ID *id, int arraysize);
extern void printsubprostart(int flag);
extern void printloadvarlocal(struct ID *id, int numflag, int adrflag,
		char *name, int writeflag);
extern void printloadvarglobal(struct ID *id, int numflag, int adrflag,
		char *name, int writeflag);

extern void printret();

extern void printassingnment();
extern int printif();
extern int printelse(int num);
extern void printelseend(int num);
extern int printwhilelabel();
extern int printwhile();
extern void printwhileend(int num1, int num2);
extern void printcall(char *proname);

extern void printread(int type);
extern void printreadend(int opr);
extern void printwrite(int type);
extern void printwriteend(int opr);

extern void printterm(int opr);
extern void printminus();
extern void printsimpleexpression(int opr);
extern void printexpression(int opr);
extern void printexception();

extern int checkvar(struct ID *id, int writeflag);
extern void setlabellist(int type, int lnumber, char* str);
extern void printcast(int type);

extern void printrefstring();
extern void printnum();
extern void printbool();
extern void printstr();
extern void printnot();
extern void printstartlabel();

extern void printpart(int type);
extern void printpush();
extern void printconvert();
