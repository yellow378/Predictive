#pragma once
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef enum {
	IF,ELSE,INT,WHILE,DO,ID,DOUBLE,VOID,RETURN,NUM,ADD,SELFADD,SUB,SELFSUB,\
	MUL,SELFMUL,DIV,SELFDIV,INC,DEC,\
	ASS,EQU,NEQU,GREAT,LESS,GREATE,LESSE,COMMA,SEMICOLON,\
	BRACEO,BRACEC,BRACKEO,BRACKEC,FANGO,FANGC,ERR,EXITCOMMEN,DOLLAR
	//{}()[]
}TokenType;
const string tokenText[] = {
	"if","else","int","while","do","ID","double","void","return","NUM","+",
	"+=","-","-=","*","*=","/","/=","++","--","=","==","!=",">","<",">=","<=",",",
	";","{","}","(",")","[","]","error","****","$"
};
typedef struct {
	TokenType type;												//记号名
	int attr;													//记号属性
	int r;		//行
	int c;		//列

}Token;
void opt(int argc, char* argv[]);								//命令行参数解析
bool isLetter(char a);							
bool isDigit(char a);
int isKey(char* buf, int start, int end);						//判断是否是关键字
int isID(char* buf, int start, int end);						//判断标识符表是否存在表项
int isNum(char* buf, int start, int end);						//判断常数表是否存在表项
void copyStr(char* target, int start, int end, char* source);	//复制
void printToken(Token token);									//打印记号
void printTable();												//打印标识符表和常数表
Token nextToken();
void error(Token token);


const int keysize = 9;
