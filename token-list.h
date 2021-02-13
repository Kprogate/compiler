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

#define NORMAL 0
#define ERROR -1

/* token-list.c */

#define KEYWORDSIZE	28

#define	TPINT			100
#define 	TPCHAR			101
#define	TPBOOL			102
#define	TPARRAY		103
#define	TPARRAYINT		104
#define 	TPARRAYCHAR	105
#define 	TPARRAYBOOL	106
#define	TPPROC			107
#define 	TPSTANDARD		108

#define	TPWORDSIZE		8

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

extern int line;

/* id-list.c */
extern struct TPW{
	int tp;
	char *tpword;
}tpw[TPWORDSIZE];

struct TYPE {
	int ttype;
	int arraysize;
	struct TYPE *etp;
	struct TYPE *paratp;
	int formal_ttype[100];
};

extern struct TYPE *tp;

struct LINE {
	int reflinenum;
	struct LINE *nextlinep;
};

extern struct LINE *globallproot, *locallproot;

struct ID {
	char *name;
	char *procname;
	struct TYPE *itp;
	int ispara;
	int definenum;
	int linenum;
	struct LINE *irefp;
	struct ID *nextp;
};

extern struct ID *globalidroot, *localidroot, *ip;
extern int formal;
extern int ttypenum;
extern int subpro;
extern int adref;

extern void init_globalidtab(void);
extern void init_localidtab(void);
extern int search_globalidtab(char *np);
extern int search_localidtab(char *np);
extern int check_globalidtab(char *np);
extern int check_localidtab(char *np);
extern int adref_globalidtab(char *np, int refline);
extern int adref_localidtab(char *np, int refline);
extern int id_countup(char *np);
extern void print_globalidtab(void);
extern void print_localidtab(void);
extern void release_globalidtab(void);
extern void release_localidtab(void);

/*実習2*/
extern char *token_str[MAXSTRSIZE];

int parse_program();
int block();
int variable_declaration();
int variable_names();
int type();
int standard_type();
int array_type();
int subprogram_declaration();
int formal_parameters();
int compound_statement();
int statement();
int condition_statement();
int iteration_statement();
int call_statement();
int expressions();
int assignment_statement();
int left_part();
int variable();
int expression();
int simple_expression();
int term();
int factor();
int input_statement();
int output_statement();
int output_format();


