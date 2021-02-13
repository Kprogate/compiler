#include "token-list.h"

//struct LINE {
//	int reflinenum;
//	struct LINE *nextlinep;
//};

//struct ID {
//	char *name;
//	char *procname;
//	struct TYPE *itp;
//	int ispara;
//	int definenum;
//	struct LINE *irefp;
//	struct ID *nextp;
//}  *globalidroot, *localidroot, *ip;

struct TPW tpw[TPWORDSIZE] = {
		{TPINT, "integer"},
		{TPCHAR, "char"},
		{TPBOOL, "boolean"},
		{TPARRAY, "array"},
		{TPARRAYINT, "of integer"},
		{TPARRAYCHAR, "of char"},
		{TPARRAYBOOL, "of boolean"},
		{TPPROC, "procedure"}
};

struct TYPE *tp;
struct LINE *globallproot, *locallproot;
struct ID *globalidroot, *localidroot, *ip;

void init_globalidtab() {		/* Initialise the table */
	globalidroot = NULL;
}

void init_localidtab() {		/* Initialise the table */
	localidroot = NULL;
}

int search_globalidtab(char *np) {	/* search the name pointed by np */
	struct ID *p;

	for(p = globalidroot; p != NULL; p = p->nextp) {
		if(strcmp(np, p->name) == 0) return(-1);
	}
	return(1);
}

int search_localidtab(char *np) {	/* search the name pointed by np */
	struct ID *p;

	for(p = localidroot; p != NULL; p = p->nextp) {
		if(strcmp(np, p->name) == 0) return(-1);		//同じ名前ある
	}
	return(1);
}

int check_globalidtab(char *np){
	struct ID *p;

	for(p = globalidroot; p != NULL; p = p->nextp) {
		if(strcmp(np, p->name) == 0){
			return(p->itp->ttype);
		}
	}
	return(error("error in check_globalidtab"));
}

int check_localidtab(char *np) {	/* search the name pointed by np */
	struct ID *p;

	for(p = localidroot; p != NULL; p = p->nextp) {
		if(strcmp(np, p->name) == 0){
			return(p->itp->paratp->ttype);
		}		//同じ名前ある
	}
	return(error("error in check_localidtab"));
}

int adref_globalidtab(char *np, int refline) {	/* search the name pointed by np */
	struct ID *p;
	struct LINE *lp;

	lp = (struct LINE *)malloc(sizeof(struct LINE));

	for(p = globalidroot; p != NULL; p = p->nextp) {
		if(strcmp(np, p->name) == 0){
			lp->reflinenum=refline;
			lp->nextlinep=p->irefp;
			p->irefp=lp;
			return NORMAL;
		}
	}
	printf("%s is not defined",np);
	return(error(""));
}

int adref_localidtab(char *np, int refline) {	/* search the name pointed by np */
	struct ID *p;
	struct LINE *lp;

	lp = (struct LINE *)malloc(sizeof(struct LINE));

	for(p = localidroot; p != NULL; p = p->nextp) {
		if(strcmp(np, p->name) == 0) {		//同じ名前ある
			lp->reflinenum=refline;
			lp->nextlinep=p->irefp;
			p->irefp=lp;
			return NORMAL;
		}
	}
	printf("%s is not defined",np);
	return(error(""));
}

int id_countup(char *np) {	/* Register and count up the name pointed by np */
	struct ID *p;
	char *cp;
	int i;

	if((p = (struct ID *)malloc(sizeof(struct ID))) == NULL) {
		return(error("can not malloc in id_countup\n"));
	}
	if((cp = (char *)malloc(strlen(np)+1)) == NULL) {
		return(error("can not malloc-2 in id_countup\n"));
	}
	if((p->itp = (struct TYPE *)malloc(sizeof(struct TYPE))) == NULL) {
		return(error("can not malloc in id_countup\n"));
	}
	if((p->itp->etp = (struct TYPE *)malloc(sizeof(struct TYPE))) == NULL) {
		return(error("can not malloc in id_countup\n"));
	}
	if((p->itp->paratp = (struct TYPE *)malloc(sizeof(struct TYPE))) == NULL) {
		return(error("can not malloc in id_countup\n"));
	}

	if((ip->procname == NULL) || ip->ispara==1){
		if(search_globalidtab(np) < 0) return(error("can not define twice"));  //二重定義
		else {
			strcpy(cp, np);
			p->name = cp;
			p->itp->ttype = tp->ttype;
			p->itp->arraysize = tp->arraysize;
			p->itp->etp->ttype = tp->etp->ttype;
			p->itp->paratp->ttype=tp->paratp->ttype;
			if(formal==1){
				for(i=0;i!=ttypenum;i++){
					p->itp->formal_ttype[i] = tp->formal_ttype[i];
				}
			}
			p->definenum = ip->definenum;
			p->procname= ip->procname;
			p->linenum=line;
			p->nextp = globalidroot;
			globalidroot = p;
		}
	}else{
		if(search_localidtab(np) < 0) return(error("can not define twice"));  //二重定義
		else {

			strcpy(cp, np);
			p->name = cp;
			p->itp->ttype = tp->ttype;
			p->itp->arraysize = tp->arraysize;
			p->itp->etp->ttype = tp->etp->ttype;
			p->itp->paratp->ttype=tp->paratp->ttype;
			p->definenum = ip->definenum;
			p->procname= ip->procname;
			p->linenum=line;
			p->nextp = localidroot;
			localidroot = p;
		}
	}
	return NORMAL;
}

void print_globalidtab() {	/* Output the registered data */
	struct ID *p;
	struct LINE *lp;
	int i;

	for(p = globalidroot; p != NULL; p = p->nextp) {
		if(p->itp->formal_ttype[0] != 0){
			printf("%-30s\t%s(", p->name, tpw[p->itp->ttype-100].tpword);
			for(i=0;p->itp->formal_ttype[i+1]!=0;i++){
				printf("%s, ",tpw[p->itp->formal_ttype[i]-100].tpword);
			}
			printf("%s",tpw[p->itp->formal_ttype[i]-100].tpword);
			printf(")\t\t\t %-4d| ",p->linenum);
			for(lp = p->irefp; lp!=NULL;lp=lp->nextlinep){
				printf("%d ",lp->reflinenum);
			}
			printf("\n");
		}else{
			printf("%-30s\t%-30s\t %-4d| ", p->name, tpw[p->itp->ttype-100].tpword, p->linenum);
			for(lp = p->irefp; lp!=NULL;lp=lp->nextlinep){
				printf("%d ",lp->reflinenum);
			}
			printf("\n");
		}
	}
}

void print_localidtab() {	/* Output the registered data */
	struct ID *p;
	struct LINE *lp;

	for(p = localidroot; p != NULL; p = p->nextp) {
		printf("%s:%-30s\t%-30s\t %-4d| ", p->name,p->procname, tpw[p->itp->paratp->ttype-100].tpword, p->linenum);
		for(lp = p->irefp; lp!=NULL;lp=lp->nextlinep){
			printf("%d ",lp->reflinenum);
		}
		printf("\n");
	}
}

void release_globalidtab() {	/* Release tha data structure */
	struct ID *p, *q;

	for(p = globalidroot; p != NULL; p = q) {
		free(p->name);
		free(p->irefp);
		q = p->nextp;
		free(p);
	}
	init_globalidtab();
}

void release_localidtab() {	/* Release tha data structure */
	struct ID *p, *q;

	for(p = localidroot; p != NULL; p = q) {
		p->name=NULL;
		p->irefp=NULL;
		q = p->nextp;
		free(p);
	}
	init_localidtab();
}
