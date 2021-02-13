#include <stdio.h>
#include "token-list.h"

#define MAXNUMSIZE 5
#define MAXNUMBER 32767

FILE *file;
int cbuf;
int num_attr;
int line=0;	//行番号
char string_attr[MAXSTRSIZE];
char number_attr[MAXNUMSIZE];

int phrase_number=0;		//単語がでてきた順番を表す番号
char *token_str[MAXSTRSIZE];

int init_scan(char *filename){

	if((file = fopen(filename, "r")) == NULL)
		return -1;

	cbuf = fgetc(file);

	return 0;

}

int scan(void){
	memset(string_attr, '\0', MAXSTRSIZE);
	memset(number_attr, '\0', MAXNUMSIZE);

	if(line==0){
		line++;		//	行番号を1からカウント
	}

		if(cbuf == EOF){
			return -1;
		}
		//空白、タブ
		else if(cbuf == 0x20 || cbuf == 0x09 ){
			cbuf = fgetc(file);
			scan();
		}
		//改行
		else if(cbuf == 0x0D || cbuf == 0x0A){
			cbuf = fgetc(file);
			if(cbuf == 0x0D || cbuf == 0x0A){
				cbuf = fgetc(file);
				line++;
			}else{
				line++;
			}
			scan();
//			return 0;
		}
		//中括弧（注釈）
		else if(cbuf == 0x7b){
//			string_attr[0]=' ';
//			token_str[phrase_number] = (char*)malloc(sizeof(char) * sizeof(string_attr));
////			token_str[phrase_number][0]=' ';
//			strcpy(token_str[phrase_number], string_attr);
//			phrase_number++;
			while((cbuf = fgetc(file)) != 0x7d){
				;
			}
			cbuf = fgetc(file);
			scan();
//			return 0;
		}
		// /*(注釈)
		else if(cbuf == 0x2f){	//cbufが/
			if((cbuf = fgetc(file))==0x2a){		//cbufが*
////				string_attr[0]=' ';
//				token_str[phrase_number] = (char*)malloc(sizeof(char) * sizeof(string_attr));
////				token_str[phrase_number][0]=' ';
//				strcpy(token_str[phrase_number], string_attr);
//				phrase_number++;
				cbuf = fgetc(file);
				if(cbuf == 0x2a){
					if((cbuf = fgetc(file))==0x2f){
						cbuf = fgetc(file);
//						return 0;
						scan();
					}
				}
				while(cbuf != 0x2a){
					cbuf = fgetc(file);
					if(cbuf == 0x2a){
						if((cbuf = fgetc(file))==0x2f)
							break;
					}
				}
				cbuf = fgetc(file);
				scan();
//				return 0;
			}else{		//条件文で次の文字を読み込んでいるからcbufを操作しなくてもいい
				return 0;		//'/'を返す
			}
		}
		//number
		else if((cbuf >=0x30)&&(cbuf <= 0x39)){
			number_attr[0]=(char)cbuf;
			cbuf = fgetc(file);
			int j;
			for(j=1;(cbuf >=0x30)&&(cbuf <= 0x39);j++){
				number_attr[j]=(char)cbuf;
				cbuf = fgetc(file);
			}
			token_str[phrase_number] = (char*)malloc(sizeof(char) * sizeof(number_attr));
			strcpy(token_str[phrase_number], number_attr);
			phrase_number++;
			num_attr = atoi(number_attr);
			if(num_attr>MAXNUMBER){		//数値が大きすぎる場合
				return -1;
			}
			return 27;		/* unsigned integer */
		}
		//String
		else if(cbuf == 0x27){
			string_attr[0]=(char)cbuf;
			cbuf = fgetc(file);
			int k;
			for(k=1; cbuf!=0x27; k++){
				string_attr[k]=(char)cbuf;
				cbuf = fgetc(file);
			}
			string_attr[k]=(char)cbuf;
			token_str[phrase_number] = (char*)malloc(sizeof(char) * sizeof(string_attr));
			strcpy(token_str[phrase_number], string_attr);
			phrase_number++;
			cbuf = fgetc(file);
			if(cbuf == 0x27){
				memset(string_attr, '\0', MAXSTRSIZE);
				string_attr[0]=(char)cbuf;
				cbuf = fgetc(file);
				for(k=1; cbuf !=0x27; k++){
					string_attr[k]=(char)cbuf;
					cbuf = fgetc(file);
				}
				string_attr[k]=(char)cbuf;
				token_str[phrase_number] = (char*)malloc(sizeof(char) * sizeof(string_attr));
				strcpy(token_str[phrase_number], string_attr);
				phrase_number++;
				cbuf = fgetc(file);
			}
			return 28;
		}
		//name, keyword
		else if(((cbuf >=0x61)&&(cbuf <= 0x7a))||((cbuf >=0x41)&&(cbuf <= 0x5a))){
			string_attr[0]=(char)cbuf;
			cbuf = fgetc(file);
			int i;
			for(i=1;((cbuf >=0x30)&&(cbuf <= 0x39))||((cbuf >=0x61)&&(cbuf <= 0x7a))||((cbuf >=0x41)&&(cbuf <= 0x5a));i++){
				string_attr[i]=(char)cbuf;
				cbuf = fgetc(file);
			}
			token_str[phrase_number] = (char*)malloc(sizeof(char) * sizeof(string_attr));
			strcpy(token_str[phrase_number], string_attr);
			phrase_number++;
			if(strcmp(string_attr,"program")==0){
				return 2;
			}else if(strcmp(string_attr,"var")==0){
				return 3;
			}else if(strncmp(string_attr,"array",4)==0){
				return 4;
			}else if(strcmp(string_attr,"of")==0){
				return 5;
			}else if(strcmp(string_attr,"begin")==0){
				return 6;
			}else if(strcmp(string_attr,"end")==0){
				return 7;
			}else if(strcmp(string_attr,"if")==0){
				return 8;
			}else if(strcmp(string_attr,"then")==0){
				return 9;
			}else if(strcmp(string_attr,"else")==0){
				return 10;
			}else if(strcmp(string_attr,"procedure")==0){
				return 11;
			}else if(strcmp(string_attr,"return")==0){
				return 12;
			}else if(strcmp(string_attr,"call")==0){
				return 13;
			}else if(strcmp(string_attr,"while")==0){
				return 14;
			}else if(strcmp(string_attr,"do")==0){
				return 15;
			}else if(strcmp(string_attr,"not")==0){
				return 16;
			}else if(strcmp(string_attr,"or")==0){
				return 17;
			}else if(strcmp(string_attr,"div")==0){
				return 18;
			}else if(strcmp(string_attr,"and")==0){
				return 19;
			}else if(strcmp(string_attr,"char")==0){
				return 20;
			}else if(strcmp(string_attr,"integer")==0){
				return 21;
			}else if(strcmp(string_attr,"boolean")==0){
				return 22;
			}else if(strcmp(string_attr,"read")==0){
				return 47;
			}else if(strcmp(string_attr,"write")==0){
				return 48;
			}else if(strcmp(string_attr,"readln")==0){
				return 23;
			}else if(strcmp(string_attr,"writeln")==0){
				return 24;
			}else if(strcmp(string_attr,"true")==0){
				return 25;
			}else if(strcmp(string_attr,"false")==0){
				return 26;
			}else if(strcmp(string_attr,"break")==0){
				return 49;
			}else{
				//追加
				if(adref==1){
				if(subpro==1){
					if(adref_localidtab(string_attr, line)==ERROR){
						exit(0);
					}
				}else{
					if(adref_globalidtab(string_attr, line)==ERROR){
						exit(0);
					}
				}
				}

				return 1;
			}

		}
		//記号
		else{
			switch(cbuf){
			case 0x2b:		//+
				string_attr[0]=(char)cbuf;
				token_str[phrase_number] = (char*)malloc(sizeof(char) * sizeof(string_attr));
				strcpy(token_str[phrase_number], string_attr);
				phrase_number++;
				cbuf = fgetc(file);
				return 29;
			case 0x2d:		//-
				string_attr[0]=(char)cbuf;
				token_str[phrase_number] = (char*)malloc(sizeof(char) * sizeof(string_attr));
				strcpy(token_str[phrase_number], string_attr);
				phrase_number++;
				cbuf = fgetc(file);
				return 30;
			case 0x2a:		//*
				string_attr[0]=(char)cbuf;
				token_str[phrase_number] = (char*)malloc(sizeof(char) * sizeof(string_attr));
				strcpy(token_str[phrase_number], string_attr);
				phrase_number++;
				cbuf = fgetc(file);
				return 31;
			case 0x3d:		//=
				string_attr[0]=(char)cbuf;
				token_str[phrase_number] = (char*)malloc(sizeof(char) * sizeof(string_attr));
				strcpy(token_str[phrase_number], string_attr);
				phrase_number++;
				cbuf = fgetc(file);
				return 32;
			case 0x3c:		//<
				string_attr[0]=(char)cbuf;
				cbuf = fgetc(file);
				if(cbuf==0x3e){
					string_attr[1]=(char)cbuf;
					token_str[phrase_number] = (char*)malloc(sizeof(char) * sizeof(string_attr));
					strcpy(token_str[phrase_number], string_attr);
					phrase_number++;
					cbuf = fgetc(file);
					return 33;		//<>
				}else if(cbuf==0x3d){
					string_attr[1]=(char)cbuf;
					token_str[phrase_number] = (char*)malloc(sizeof(char) * sizeof(string_attr));
					strcpy(token_str[phrase_number], string_attr);
					phrase_number++;
					cbuf = fgetc(file);
					return 35;		//<=
				}else{
					token_str[phrase_number] = (char*)malloc(sizeof(char) * sizeof(string_attr));
					strcpy(token_str[phrase_number], string_attr);
					phrase_number++;
					return 34;		//<
				}
			case 0x3e:		//>
				string_attr[0]=(char)cbuf;
				cbuf = fgetc(file);
				if(cbuf==0x3d){
					string_attr[1]=(char)cbuf;
					token_str[phrase_number] = (char*)malloc(sizeof(char) * sizeof(string_attr));
					strcpy(token_str[phrase_number], string_attr);
					phrase_number++;
					cbuf = fgetc(file);
					return 37;		//>=
				}else{
					token_str[phrase_number] = (char*)malloc(sizeof(char) * sizeof(string_attr));
					strcpy(token_str[phrase_number], string_attr);
					phrase_number++;
					return 36;		//>
				}
			case 0x28:		//(
				string_attr[0]=(char)cbuf;
				token_str[phrase_number] = (char*)malloc(sizeof(char) * sizeof(string_attr));
				strcpy(token_str[phrase_number], string_attr);
				phrase_number++;
				cbuf = fgetc(file);
				return 38;
			case 0x29:		//)
				string_attr[0]=(char)cbuf;
				token_str[phrase_number] = (char*)malloc(sizeof(char) * sizeof(string_attr));
				strcpy(token_str[phrase_number], string_attr);
				phrase_number++;
				cbuf = fgetc(file);
				return 39;
			case 0x5b:		//[
				string_attr[0]=(char)cbuf;
				token_str[phrase_number] = (char*)malloc(sizeof(char) * sizeof(string_attr));
				strcpy(token_str[phrase_number], string_attr);
				phrase_number++;
				cbuf = fgetc(file);
				return 40;
			case 0x5d:		//]
				string_attr[0]=(char)cbuf;
				token_str[phrase_number] = (char*)malloc(sizeof(char) * sizeof(string_attr));
				strcpy(token_str[phrase_number], string_attr);
				phrase_number++;
				cbuf = fgetc(file);
				return 41;
			case 0x3a:		//:
				string_attr[0]=(char)cbuf;
				cbuf = fgetc(file);
				if(cbuf==0x3d){
					string_attr[1]=(char)cbuf;
					token_str[phrase_number] = (char*)malloc(sizeof(char) * sizeof(string_attr));
					strcpy(token_str[phrase_number], string_attr);
					phrase_number++;
					cbuf = fgetc(file);
					return 42;		//:=
				}else{
					token_str[phrase_number] = (char*)malloc(sizeof(char) * sizeof(string_attr));
					strcpy(token_str[phrase_number], string_attr);
					phrase_number++;
					return 45;		//:
				}
			case 0x2e:		//.
				string_attr[0]=(char)cbuf;
				token_str[phrase_number] = (char*)malloc(sizeof(char) * sizeof(string_attr));
				strcpy(token_str[phrase_number], string_attr);
				phrase_number++;
				cbuf = fgetc(file);
				return 43;
			case 0x2c:		//,
				string_attr[0]=(char)cbuf;
				token_str[phrase_number] = (char*)malloc(sizeof(char) * sizeof(string_attr));
				strcpy(token_str[phrase_number], string_attr);
				phrase_number++;
				cbuf = fgetc(file);
				return 44;
			case 0x3b:		//;
				string_attr[0]=(char)cbuf;
				token_str[phrase_number] = (char*)malloc(sizeof(char) * sizeof(string_attr));
				strcpy(token_str[phrase_number], string_attr);
				phrase_number++;
				cbuf = fgetc(file);
				return 46;
			default:
				string_attr[0]=(char)cbuf;
				token_str[phrase_number] = (char*)malloc(sizeof(char) * sizeof(string_attr));
				strcpy(token_str[phrase_number], string_attr);
				phrase_number++;
				cbuf = fgetc(file);
				return 0;
			}
		}
}

int get_linenum(void){
	return line;
}

void end_scan(void){
	fclose(file);
}
