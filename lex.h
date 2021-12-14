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
	TokenType type;												//�Ǻ���
	int attr;													//�Ǻ�����
	int r;		//��
	int c;		//��

}Token;
void opt(int argc, char* argv[]);								//�����в�������
bool isLetter(char a);							
bool isDigit(char a);
int isKey(char* buf, int start, int end);						//�ж��Ƿ��ǹؼ���
int isID(char* buf, int start, int end);						//�жϱ�ʶ�����Ƿ���ڱ���
int isNum(char* buf, int start, int end);						//�жϳ������Ƿ���ڱ���
void copyStr(char* target, int start, int end, char* source);	//����
void printToken(Token token);									//��ӡ�Ǻ�
void printTable();												//��ӡ��ʶ����ͳ�����
Token nextToken();
void error(Token token);


const int keysize = 9;
