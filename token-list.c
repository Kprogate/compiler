#include "token-list.h"

/* keyword list */
struct KEY key[KEYWORDSIZE] = {
	{"and", 	TAND	},
	{"array",	TARRAY	},
	{"begin",	TBEGIN	},
	{"boolean",	TBOOLEAN},
	{"break",	TBREAK  },
	{"call",	TCALL	},
	{"char",	TCHAR	},
	{"div",		TDIV	},
	{"do",		TDO	},
	{"else",	TELSE	},
	{"end",		TEND	},
	{"false",	TFALSE	},
	{"if",		TIF	},
	{"integer",	TINTEGER},
	{"not",		TNOT	},
	{"of",		TOF	},
	{"or",		TOR	},
	{"procedure", TPROCEDURE},
	{"program",	TPROGRAM},
	{"read",	TREAD	},
	{"readln",	TREADLN },
	{"return", 	TRETURN },
	{"then",	TTHEN	},
	{"true",	TTRUE	},
	{"var",		TVAR	},
	{"while",	TWHILE	},
	{"write",	TWRITE  },
	{"writeln",	TWRITELN}
};


/* Token counter */
int numtoken[NUMOFTOKEN+1];

int token;
int iteration=0;
int exit_flag=0;
int tab=0;
int subpro=0;
int adref=0;		//変数定義中かどうかのフラグ

int global=0; //グローバル変数かどうか
char *names[MAXSTRSIZE];		//変数名の並びを保存
char procname[MAXSTRSIZE];
int dnum=0;	//変数の定義を何個記憶させるか
int ttypenum=0;
int formal=0;
int tpint=0; 	//単純式の項に+-があるかのフラグ
int tpexp=0;	//式が単純式1つで構成されているかどうかのフラグ

//それぞれの関数が返す型
int tp_lef;
int tp_var;
int tp_exp;
int tp_sim;
int tp_ter;
int tp_fac;

/* string of each token */
char *tokenstr[NUMOFTOKEN+1] = {
	"",
	"NAME", "program", "var", "array", "of", "begin", "end", "if", "then",
	 "else", "procedure", "return", "call", "while", "do", "not", "or", 
	"div", "and", "char", "integer", "boolean", "readln", "writeln", "true",
	 "false", "NUMBER", "STRING", "+", "-", "*", "=", "<>", "<", "<=", ">", 
	">=", "(", ")", "[", "]", ":=", ".", ",", ":", ";", "read","write", "break"
};

int main(int nc, char *np[]) {
//	int i,j;

    if(nc < 2) {
	printf("File name id not given.\n");
	return 0;
    }
    if(init_scan(np[1]) < 0) {
	printf("File %s can not open.\n", np[1]);
	return 0;
    }

	if((ip = (struct ID *)malloc(sizeof(struct ID))) == NULL) {
		printf("can not malloc\n");
	}
    ip->definenum=0;

	if((tp = (struct TYPE *)malloc(sizeof(struct TYPE))) == NULL) {
		printf("can not malloc\n");
	}
	if((tp->etp = (struct TYPE *)malloc(sizeof(struct TYPE))) == NULL) {
		printf("can not malloc\n");
	}
	if((tp->paratp = (struct TYPE *)malloc(sizeof(struct TYPE))) == NULL) {
		printf("can not malloc\n");
	}

	printf("Name\t\t\t\t\tType\t\t\t\t\tDef.  Ref.\n");

    /* 作成する部分：トークンカウント用の配列？を初期化する */
//    for(i=0;i<NUMOFTOKEN+1;i++){
//    	numtoken[i] = 0;
//    }

//    while((token = scan()) >= 0) {
//	/* 作成する部分：トークンをカウントする */
////    	numtoken[token]++;
//
//    	/*tokenに応じて分の処理が決まる、switch文で場合分け*/
//    }

    token=scan();
    if(parse_program()==ERROR)
    	return -1;
//    parse_program();

//    end_scan();

    /* 作成する部分:カウントした結果を出力する */
//    for(i=1;i<NUMOFTOKEN+1;i++){
//    	if(numtoken[i]==0){
//    		continue;
//    	}else{
//    		printf("\"%s\"  %d\n",tokenstr[i],numtoken[i]);
//    	}
//    }

    //プリティプリント
//    for(i=0; token_str[i+1]!=NULL;i++){
//    	printf("%s ", token_str[i]);
//    	if(strcmp(token_str[i+1],"procedure")==0){
//    		tab=1;
//    		procedure_flag=1;
//    	}
//    	if(strcmp(token_str[i],";")==0){
//    		printf("\n");
//    		if(strcmp(token_str[i+1],"end")==0){
//    			tab--;
//    		}
//    		for(j=0; j<tab;j++){
//    			printf("\t");
//    		}
//    		if(strcmp(token_str[i+1],"end")==0){
//    			if(tab==1 && procedure_flag==1){
//    				tab=0;
//    				procedure_flag=0;
//    			}
//    		}
//    	}else if(strcmp(token_str[i+1],"end")==0){		//endの直前に';'が無い場合
//    		printf("\n");
//    		tab--;
//    		for(j=0; j<tab;j++){
//    			printf("\t");
//    		}
//    		if(tab==1 && procedure_flag==1){
//				tab=0;
//				procedure_flag=0;
//    		}
//    	}else if(strcmp(token_str[i+1],"else")==0){		//elseの直前に';'が無い場合
//    		printf("\n");
//    		for(j=0; j<tab;j++){
//    			printf("\t");
//    		}
//    	}
//    	if(strcmp(token_str[i+1],"var")==0){
//    		if(tab<1)
//    			printf("\t");
//    	}
//    	if(strcmp(token_str[i],"begin")==0){
//    		printf("\n");
//    		tab++;
//    		for(j=0; j<tab;j++){
//    			printf("\t");
//    		}
//    	}
//    	if(strcmp(token_str[i],"then")==0 || strcmp(token_str[i],"else")==0 || strcmp(token_str[i],"do")==0){
//    		printf("\n");
//    		for(j=0; j<tab;j++){
//    			printf("\t");
//    		}
//    		if(strcmp(token_str[i+1],"begin")!=0){		//分岐文、繰り返し文の中身が複合文ではないとき
//        		printf("\t");
//    		}
//    	}
//    }
//    printf("%s ", token_str[i]);
//    printf("\n");

//    for(i=0;names[i]!=NULL;i++){
//    	printf("%s ",names[i]);
//    }
//    id_countup("ab",NULL,5);
//    id_countup("cd","a",5);
    print_globalidtab();

    return 0;
}

int error(char *mes) {
	printf("\n ERROR in %d: %s\n",get_linenum(), mes);
	end_scan();
	return ERROR;
}

int parse_program(){
	if(token!=TPROGRAM)
		return(error("Keyword 'program' is not found"));
	token = scan();
	if(token != TNAME)
		return(error("Program-name is not found"));
	token=scan();
	if(token != TSEMI)
		return(error("Semicolon is not found"));
	token=scan();
	if(block() == ERROR)
		return(ERROR);
	if(token!=TDOT)
		return(error("Period is not found at the end of program"));
	token=scan();
	return(NORMAL);
}

int block(){
	while(token == TVAR || token == TPROCEDURE){
		switch(token){
		case TVAR:
			global = 1;
			if(variable_declaration()==ERROR)
				return ERROR;
			break;
		case TPROCEDURE:
			if(subprogram_declaration()==ERROR)
				return ERROR;
			break;
		}
	}
	adref=1;
	if(compound_statement()==ERROR)
		return ERROR;
	adref=0;
	return(NORMAL);
}

int variable_declaration(){
	if(token!=TVAR)
		return(error("Keyword 'var' is not found"));
	token=scan();
	if(variable_names()==ERROR)
		return ERROR;
	if(token!=TCOLON)
		return(error("Colon is not found"));
	token=scan();
	if(type()==ERROR)
		return ERROR;
	while(dnum!=ip->definenum){
		if(id_countup(names[dnum])==ERROR)
			return ERROR;
		dnum++;
	}
	if(token!=TSEMI)
		return(error("Semicolon is not found"));
	token=scan();
	while(token==TNAME){
		if(variable_names()==ERROR)
			return ERROR;
		if(token!=TCOLON)
			return(error("Colon is not found"));
		token=scan();
		if(type()==ERROR)
				return ERROR;
		while(dnum!=ip->definenum){
			if(id_countup(names[dnum])==ERROR)
				return ERROR;
			dnum++;
		}
		if(token!=TSEMI)
			return(error("Semicolon is not found"));
		token=scan();
	}
	return(NORMAL);
}

int variable_names(){
	if(token!=TNAME)
		return(error("Variable-name is not found"));
	names[ip->definenum]=(char*)malloc(sizeof(char) * sizeof(string_attr));
	strcpy(names[ip->definenum], string_attr);
	ip->definenum++;
	token=scan();
	while(token==TCOMMA){
		if(token!=TCOMMA)
			return(error("Comma is not found"));
		token=scan();
		if(token!=TNAME)
			return(error("Variable-name is not found"));
		names[ip->definenum]=(char*)malloc(sizeof(char) * sizeof(string_attr));
		strcpy(names[ip->definenum], string_attr);
		ip->definenum++;
		token=scan();
	}
	return(NORMAL);
}

int type(){
	switch(token){
	case TINTEGER:
		if(standard_type()==ERROR)
			return ERROR;
		break;
	case TBOOLEAN:
		if(standard_type()==ERROR)
			return ERROR;
		break;
	case TCHAR:
		if(standard_type()==ERROR)
			return ERROR;
		break;
	case TARRAY:
		tp->ttype = TPARRAY;
		if(array_type()==ERROR)
			return ERROR;
		break;
	default:
		return(error("Type is not found"));
	}
	return(NORMAL);
}

int standard_type(){
	if((token!=TINTEGER)&&(token!=TBOOLEAN)&&(token!=TCHAR))
		return(error("Keyword 'integer' or 'boolean' or 'char' is not found"));
	switch(token){
	case TINTEGER:
		if(tp->ttype == TPARRAY){
			tp->etp->ttype = TPINT;
		}else if(tp->ttype == TPPROC){
			if(formal==1){
				tp->formal_ttype[ttypenum]=TPINT;
				ttypenum++;
			}
			tp->paratp->ttype = TPINT;
		}else{
			tp->ttype = TPINT;
		}
		break;
	case TBOOLEAN:
		if(tp->ttype == TPARRAY){
			tp->etp->ttype = TPBOOL;
		}else if(tp->ttype == TPPROC){
			if(formal==1){
				tp->formal_ttype[ttypenum]=TPBOOL;
				ttypenum++;
			}
			tp->paratp->ttype = TPBOOL;
		}else{
			tp->ttype = TPBOOL;
		}
		break;
	case TCHAR:
		if(tp->ttype == TPARRAY){
			tp->etp->ttype = TPCHAR;
		}else if(tp->ttype == TPPROC){
			if(formal==1){
				tp->formal_ttype[ttypenum]=TPCHAR;
				ttypenum++;
			}
			tp->paratp->ttype = TPCHAR;	//配列にして記憶
		}else{
			tp->ttype = TPCHAR;
		}
		break;
	}
	token=scan();
	return NORMAL;
}

int array_type(){
	if(token!=TARRAY)
		return(error("Keyword 'array' is not found"));
	token=scan();
	if(token!=TLSQPAREN)
		return(error("Left-square-paren is not found"));
	token=scan();
	if(token!=TNUMBER)
		return(error("Number is not found"));
	tp->arraysize = num_attr;
	token=scan();
	if(token!=TRSQPAREN)
		return(error("Right-square-paren is not found"));
	token =scan();
	if(token!=TOF)
		return(error("Keyword 'of' is not found"));
	token=scan();
	if(standard_type()==ERROR)
		return ERROR;
	return NORMAL;
}

int subprogram_declaration(){
	if(token!=TPROCEDURE)
		return(error("Keyword 'procedure' is not found"));
	tp->ttype=TPPROC;
	subpro=1;
	token=scan();
	if(token!=TNAME)
		return(error("Procedure-name is not found"));
	names[ip->definenum]=(char*)malloc(sizeof(char) * sizeof(string_attr));
	ip->procname=(char*)malloc(sizeof(char) * sizeof(string_attr));
	strcpy(names[ip->definenum], string_attr);
	strcpy(ip->procname, string_attr);
	ip->definenum++;
	token=scan();
	if(token==TLPAREN){
		formal=1;
		if(formal_parameters()==ERROR)
			return ERROR;
	}
	int i=0;
	while(dnum!=ip->definenum){
		if(i==0){
			ip->ispara=1;
		}else{
			ip->ispara=0;
		}
		if(id_countup(names[dnum])==ERROR)
			return ERROR;
	dnum++;
	i++;
	}
	formal=0;
	ttypenum=0;
	memset(tp->formal_ttype,0,100);
	if(token!=TSEMI)
		return(error("Semicolon is not found"));
	token=scan();
	if(token==TVAR){
		if(variable_declaration()==ERROR)
			return ERROR;
	}
	adref=1;
	if(compound_statement()==ERROR)
		return ERROR;
	adref=0;
	if(token!=TSEMI)
		return(error("Semicolon is not found"));
	token=scan();


	print_localidtab();
	release_localidtab();
	ip->procname=NULL;
	tp->ttype=0;
	tp->paratp->ttype=0;
	subpro=0;
	return NORMAL;
}

int formal_parameters(){
	if(token!=TLPAREN)
		return(error("Left-paren is not found"));
	token=scan();
	if(variable_names()==ERROR)
		return ERROR;
	if(token!=TCOLON)
		return(error("Colon is not found"));
	token = scan();
	if(type()==ERROR)
		return ERROR;
	while(token==TSEMI){
		if(token!=TSEMI)
			return(error("Semicolon is not found"));
		token=scan();
		if(variable_names()==ERROR)
			return ERROR;
		if(token!=TCOLON)
			return(error("Colon is not found"));
		token=scan();
		if(type()==ERROR)
			return ERROR;
	}
	if(token!=TRPAREN)
		return(error("Right-paren is not found"));
	token=scan();
	return NORMAL;
}

int compound_statement(){
	if(token!=TBEGIN)
		return(error("Keyword 'begin' is not found"));
	token=scan();
	if(statement()==ERROR)
		return ERROR;
	while(token==TSEMI){
		token=scan();
		if(statement()==ERROR)
			return ERROR;
	}
	if(token!=TEND)
		return(error("Keyword 'end' is not found"));
	token=scan();
	return NORMAL;
}

int statement(){
	switch(token){
	case TNAME:
		if(assignment_statement()==ERROR)
			return ERROR;
		break;
	case TIF:
		if(condition_statement()==ERROR)
			return ERROR;
		break;
	case TWHILE:
		if(iteration_statement()==ERROR)
			return ERROR;
		break;
	case TBREAK:
		if(exit_flag!=1)
			return(error("Keyword 'break' is not found in iteration-statement"));
		token=scan();
		return NORMAL;
		break;
	case TCALL:
		if(call_statement()==ERROR)
			return ERROR;
		break;
	case TRETURN:
		token=scan();
		return NORMAL;
		break;
	case TREAD:
	case TREADLN:
		if(input_statement()==ERROR)
			return ERROR;
		break;
	case TWRITE:
	case TWRITELN:
		if(output_statement()==ERROR)
			return ERROR;
		break;
	case TBEGIN:
		if(compound_statement()==ERROR)
			return ERROR;
		break;
	default:		//空文はエラーではないから
		return NORMAL;
	}
	return NORMAL;	//空文はエラーではないから
}

int condition_statement(){
	if(token!=TIF)
		return(error("Keyword 'if' is not found"));
	token=scan();
	if(expression()==ERROR)
		return ERROR;
	if(tp_exp!= TPBOOL){
		return(error("type is not corrected"));
	}
	if(token!=TTHEN)
		return(error("Keyword 'then' is not found"));
	token=scan();
	if(statement()==ERROR)
		return ERROR;
	if(token==TELSE){
		token=scan();
		if(statement()==ERROR)
			return ERROR;
	}
	return NORMAL;
}

int iteration_statement(){
	iteration++;
	exit_flag=1;
	if(token!=TWHILE)
		return(error("Keyword 'while' is not found"));
	token=scan();
	if(expression()==ERROR)
		return ERROR;
	if(tp_exp!= TPBOOL){
		return(error("type is not corrected"));
	}
	if(token!=TDO)
		return(error("Keyword 'do' is not found"));
	token=scan();
	if(statement()==ERROR)
		return ERROR;
//	if(exit_flag!=1){
//		if(iteration==1)
//			return(error("Exit-statement is not found"));
//	}
	iteration--;
	if(iteration==0)
		exit_flag=0;
	return NORMAL;
}

int call_statement(){
	if(token!=TCALL)
		return(error("Keyword 'call' is not found"));
	token=scan();
	if(token!=TNAME)
		return(error("Procedure-name is not found"));
	token=scan();
	if(token==TLPAREN){
		token=scan();
		if(expressions()==ERROR)
			return ERROR;
		if(token!=TRPAREN)
			return(error("Right-paren is not found"));
		token=scan();
	}
	return NORMAL;
}

int expressions(){
	if(expression()==ERROR)
		return ERROR;
	while(token==TCOMMA){
		token=scan();
		if(expression()==ERROR)
			return ERROR;
	}
	return NORMAL;
}

int assignment_statement(){
	if(left_part()==ERROR)
		return ERROR;
	if(token!=TASSIGN)
		return(error("Assign is not found"));
	token=scan();
	if(expression()==ERROR)
		return ERROR;
	if(tp_lef!=tp_exp){
		return(error("assignment is not corrected"));
	}
	if(tp_lef!=TPINT && tp_lef!=TPBOOL && tp_lef!=TPCHAR){
		return(error("assignment is not corrected"));
	}
	return NORMAL;
}

int left_part(){
	if(variable()==ERROR)
		return ERROR;
	tp_lef=tp_var;
	return NORMAL;
}

int variable(){
	if(token!=TNAME)
		return(error("Variable-name is not found"));
	if(adref==1){
	if(subpro==1){
		tp_var=check_localidtab(string_attr);
	}else{
		tp_var=check_globalidtab(string_attr);
	}
	}
	token=scan();
	if(token==TLSQPAREN){
		if(tp_var != TPARRAY){
			return(error("type is not corrected"));
		}
		token=scan();
		if(expression()==ERROR)
			return ERROR;
		if(tp_exp != TPINT){
			return(error("type is not corrected"));
		}
		if(token!=TRSQPAREN)
			return(error("Right-square-paren is not found"));
		token=scan();
	}
	return NORMAL;
}

int expression(){
	if(simple_expression()==ERROR)
		return ERROR;
	tp_exp=tp_sim;
	while(token==TEQUAL || token==TNOTEQ || token==TLE || token==TLEEQ || token==TGR || token==TGREQ){
		tpexp=1;
		token=scan();
		if(simple_expression()==ERROR)
			return ERROR;
		if(tp_sim!=tp_exp){
			return(error("type is not corrected"));
		}
		if(tp_sim!=TPINT && tp_sim!=TPBOOL && tp_sim!=TPCHAR){
			return(error("type is not corrected"));
		}
	}
	if(tpexp==1){
		tp_exp=TPBOOL;
		tpexp=0;
	}
	return NORMAL;
}

int simple_expression(){
	if(token==TPLUS || token==TMINUS){
		tpint=1;
		token=scan();
	}
	if(term()==ERROR)
		return ERROR;
	if(tpint==1){
		if(tp_ter != TPINT){
			return(error("type is not corrected"));
		}
		tpint=0;
	}
	tp_sim=tp_ter;
	while(token==TPLUS || token==TMINUS || token==TOR){
		switch(token){
		case TPLUS:
		case TMINUS:
			token=scan();
			if(term()==ERROR)
				return ERROR;
			if(tp_ter != TPINT){
				return(error("type is not corrected"));
			}
			tp_sim=TPINT;
			break;
		case TOR:
			token=scan();
			if(term()==ERROR)
				return ERROR;
			if(tp_ter != TPBOOL){
				return(error("type is not corrected"));
			}
			tp_sim=TPBOOL;
			break;
		}
	}
	return NORMAL;
}

int term(){
	if(factor()==ERROR)
		return ERROR;
	tp_ter=tp_fac;
	while(token==TSTAR || token==TDIV || token==TAND){
		switch(token){
		case TSTAR:
		case TDIV:
			token=scan();
			if(factor()==ERROR)
				return ERROR;
			if(tp_fac != TPINT){
				return(error("type is not corrected"));
			}
			tp_ter=TPINT;
			break;
		case TAND:
			token=scan();
			if(factor()==ERROR)
				return ERROR;
			if(tp_fac != TPBOOL){
				return(error("type is not corrected"));
			}
			tp_ter=TPBOOL;
			break;
		}
	}
	return NORMAL;
}

int factor(){
	switch(token){
	case TNAME:
		if(variable()==ERROR)
			return ERROR;
		tp_fac=tp_var;
		break;
	case TNUMBER:
		tp_fac=TPINT;
		token=scan();
		return NORMAL;
		break;
	case TFALSE:
	case TTRUE:
		tp_fac=TPBOOL;
		token=scan();
		return NORMAL;
		break;
	case TSTRING:
		if(strlen(string_attr)!=3){
			return(error("fixed number in factor should be one character"));
		}
		tp_fac=TPCHAR;
		token=scan();
		return NORMAL;
		break;
	case TLPAREN:
		token=scan();
		if(expression()==ERROR)
			return ERROR;
		tp_fac=tp_exp;
		if(token!=TRPAREN)
			return(error("Right-paren is not found"));
		token=scan();
		break;
	case TNOT:
		token=scan();
		if(factor()==ERROR)
			return ERROR;
		if(tp_fac!=TPBOOL){
			return(error("type is not corrected"));
		}
		tp_fac=TPBOOL;
		break;
	case TINTEGER:
		tp_fac=TPINT;
		token=scan();
		if(token!=TLPAREN)
			return(error("Left-paren is not found"));
		token=scan();
		if(expression()==ERROR)
			return ERROR;
		if(tp_exp!=TPINT && tp_exp!=TPBOOL && tp_exp!=TPCHAR){
			return(error("type is not corrected"));
		}
		if(token!=TRPAREN)
			return(error("Right-paren is not found"));
		token=scan();
		break;
	case TBOOLEAN:
		tp_fac=TPBOOL;
		token=scan();
		if(token!=TLPAREN)
			return(error("Left-paren is not found"));
		token=scan();
		if(expression()==ERROR)
			return ERROR;
		if(tp_exp!=TPINT && tp_exp!=TPBOOL && tp_exp!=TPCHAR){
			return(error("type is not corrected"));
		}
		if(token!=TRPAREN)
			return(error("Right-paren is not found"));
		token=scan();
		break;
	case TCHAR:
		tp_fac=TPCHAR;
		token=scan();
		if(token!=TLPAREN)
			return(error("Left-paren is not found"));
		token=scan();
		if(expression()==ERROR)
			return ERROR;
		if(tp_exp!=TPINT && tp_exp!=TPBOOL && tp_exp!=TPCHAR){
			return(error("type is not corrected"));
		}
		if(token!=TRPAREN)
			return(error("Right-paren is not found"));
		token=scan();
		break;
	default:
		return(error("Factor is not found"));
	}
	return NORMAL;
}

int input_statement(){
	if(token!=TREAD && token!=TREADLN)
		return(error("Keyword 'read' or 'readln' is not found"));
	token=scan();
	if(token==TLPAREN){
		token=scan();
		if(variable()==ERROR)
			return ERROR;
		if(tp_var!=TPINT && tp_var!=TPCHAR){
			return(error("type is not corrected"));
		}
		while(token == TCOMMA){
			token=scan();
			if(variable()==ERROR)
				return ERROR;
			if(tp_var!=TPINT && tp_var!=TPCHAR){
				return(error("type is not corrected"));
			}
		}
		if(token!=TRPAREN)
			return(error("Right-paren is not found"));
		token=scan();
	}
	return NORMAL;
}

int output_statement(){
	if(token!=TWRITE && token!=TWRITELN)
			return(error("Keyword 'write' or 'writeln' is not found"));
		token=scan();
		if(token==TLPAREN){
			token=scan();
			if(output_format()==ERROR)
				return ERROR;
			while(token == TCOMMA){
				token=scan();
				if(output_format()==ERROR)
					return ERROR;
			}
			if(token!=TRPAREN)
				return(error("Right-paren is not found"));
			token=scan();
		}
	return NORMAL;
}

int output_format(){
	switch(token){
	case TSTRING:		//式ではない文字列
		if(strlen(string_attr)!=1){
			token=scan();
			break;
		}
	case TPLUS:
	case TMINUS:
	case TNAME:
	case TNUMBER:
	case TFALSE:
	case TTRUE:
	case TLPAREN:
	case TNOT:
	case TINTEGER:
	case TBOOLEAN:
	case TCHAR:
		if(expression()==ERROR)
			return ERROR;
		if(tp_exp!=TPINT && tp_exp!=TPBOOL && tp_exp!=TPCHAR){
			return(error("type is not corrected"));
		}
		if(token==TCOLON){
			token=scan();
			if(token!=TNUMBER)
				return(error("Number is not found"));
			token=scan();
		}
		break;
	default:
		return(error("Output-format is not found"));
	}
	return NORMAL;
}


