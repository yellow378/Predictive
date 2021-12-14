#include "predictive.h"
#include "lex.h"
extern Token curToken;
extern char buff[1024];												//缓冲区
extern int buffSize;
extern int curPos;														//当前位置
extern int curRow;
extern char textPath[100];												//输入如文件路径
extern char outPath[100];										//输出文件路径
extern char keyTable[20][20];	//关键字表
extern const int keysize;											//关键字个数
extern char idTable[100][10];											//标识符表
extern int idsize;													//符号表表项数
extern char numTable[100][10];											//常量表
extern int numsize;												//常数表表项数

extern bool fileOut;											//是否有输出文件
extern FILE* of, * f;													//输入输出文件

int over;
void opt(int argc, char* argv[]) {
	if (argc == 1) {
		printf("请输入代码文件路径:");
		scanf("%s", textPath);			//没有输入文件，终端输入
	}
	else if (argc == 2) {
		strcpy(textPath, argv[1]);		//只有输入文件
	}
	else if (argc == 4) {
		if (strcmp(argv[1], "-out") == 0) {
			strcpy(outPath, argv[2]);
			strcpy(textPath, argv[3]);
		}
		else if (strcmp(argv[2], "-out") == 0) {
			strcpy(outPath, argv[3]);
			strcpy(textPath, argv[1]);
		}
		of = fopen(outPath, "w");		//有输出文件，打开
		fileOut = true;
	}
}
bool isLetter(char a) {
	if (a <= 'z' && a >= 'a' || a <= 'Z' && a >= 'A')
		return true;
	else return false;
}
bool isDigit(char a) {
	if (a <= '9' && a >= '0') return true;
	else return false;
}
int isKey(char* buf, int start, int end) {
	char cmpstr[20];
	copyStr(cmpstr, start, end, buf);
	for (int i = 0; i < keysize; i++) {
		if (strcmp(cmpstr, keyTable[i]) == 0) {
			return i;
		}
	}
	return -1;
}
int isID(char* buf, int start, int end) {
	char cmpstr[20];
	copyStr(cmpstr, start, end, buf);
	for (int i = 0; i < idsize; i++) {
		if (strcmp(cmpstr, idTable[i]) == 0) {
			return i;
		}
	}
	return -1;
}
void copyStr(char* target, int start, int end, char* source) {
	for (int i = start; i <= end; i++) {
		*(target++) = *(source + i);
	}
	*(target++) = '\0';
}
void printToken(Token token) {
	if (fileOut) {
		if (token.type == ERR) {
			fprintf(of, "Error Symbol\n");
		}
		else {
			fprintf(of, "(%d,%d)\t\t", token.type, token.attr);
			fprintf(of, "行: %d, 列: %d\n", token.r, token.c);
		}
	}
	else {
		if (token.type == ERR) {
			printf("Error Symbol\n");
		}
		else {
			printf("(%d,%d)\t\t", token.type, token.attr);
			printf("行: %d, 列: %d\n", token.r, token.c);
		}

	}
}
int isNum(char* buf, int start, int end) {
	char cmpstr[20];
	copyStr(cmpstr, start, end, buf);
	for (int i = 0; i < numsize; i++) {
		if (strcmp(cmpstr, numTable[i]) == 0) {
			return i;
		}
	}
	return -1;
}
void printTable() {
	//printf("标识符符号表\n");
	//printf("-------------\n");
	//for (int i = 0; i < idsize; i++) {
	//	printf("%2d | %s\n", i, idTable[i]);
	//}
	//printf("-------------\n");
	//printf("常量符号表\n");
	//printf("-------------\n");
	//for (int i = 0; i < numsize; i++) {
	//	printf("%2d | %s\n", i, numTable[i]);
	//}
	//printf("-------------\n");
	if (fileOut) {
		fprintf(of, "标识符符号表\n");
		fprintf(of, "-------------\n");
		for (int i = 0; i < idsize; i++) {
			fprintf(of, "%2d | %s\n", i, idTable[i]);
		}
		fprintf(of, "-------------\n");
		fprintf(of, "常量符号表\n");
		fprintf(of, "-------------\n");
		for (int i = 0; i < numsize; i++) {
			fprintf(of, "%2d | %s\n", i, numTable[i]);
		}
		fprintf(of, "-------------\n");

	}
}
Token nextToken() {
	Token token = { ERR,0 };
	int i, j;
	char mask;
	i = j = curPos;
	if (buffSize <= 0) {
		over = fscanf(f, "%[^\n]", buff);
		//printf("%s\n", buff);
		fscanf(f, "%c", &mask);
		buffSize = strlen(buff);
		i = j = curPos = 0;
		curRow++;
		if (over == EOF || over == 0) {
			token.type = DOLLAR;
			token.attr = 0;
			return token;
		}
	}
	if (buff[j] == ' ') {
		while (buff[j] == ' ') {
			j++;
			buffSize--;
		}
		i = j;
		curPos = i;
		return nextToken();
	}
	if (isLetter(buff[j])) {
		j++;
		while (isLetter(buff[j]) || isDigit(buff[j])) {
			j++;
		}
		j--;			//确定范围
		int keycheck = isKey(buff, i, j);
		if (keycheck == -1) {				//判断不为关键字，即标识符
			token.type = ID;
			int idcheck = isID(buff, i, j);	//判断标识符表中是已有声明
			if (idcheck == -1) {
				copyStr(idTable[idsize], i, j, buff);	//没有声明，则添加进标识符表
				token.attr = idsize++;
			}
			else {
				token.attr = idcheck;		//已有声明，属性设置为表中下标
			}
		}
		else {
			token.type = (TokenType)keycheck;	//判断为关键字，记号设置为对应关键字
			token.attr = 0;
		}
	}
	else if (isDigit(buff[j])) {
		bool flag = true;
		j++;
		while (isDigit(buff[j]) || (flag && buff[j] == '.')) {
			if (buff[j] == '.') flag = false;
			j++;
		}
		j--;		//确定范围
		token.type = NUM;	//记号名为NUM
		int numcheck = isNum(buff, i, j);	//确定表中是否已有相同的项
		if (numcheck == -1) {
			copyStr(numTable[numsize], i, j, buff);	//不在表中新建
			token.attr = numsize++;
		}
		else token.attr = numcheck;					//在表中，属性为表中的下标
	}
	else if (buff[j] == '+')
	{
		j++;
		if (buff[j] == '=') {
			token.type = SELFADD;					//+=
		}
		else if (buff[j] == '+') {
			token.type = INC;						//++
		}
		else {
			j--;
			token.type = ADD;						//+
		}
		token.attr = 0;
	}
	else if (buff[j] == '-')
	{
		j++;
		if (buff[j] == '=') {
			token.type = SELFSUB;
		}
		else if (buff[j] == '-') {
			token.type = DEC;
		}
		else {
			j--;
			token.type = SUB;
		}
		token.attr = 0;
	}
	else if (buff[j] == '*')
	{
		j++;
		if (buff[j] == '=') {
			token.type = SELFMUL;
		}
		else if (buff[j] == '/') {
			token.type = EXITCOMMEN;
		}
		else {
			j--;
			token.type = MUL;
		}
		token.attr = 0;
	}
	else if (buff[j] == '/')
	{
		j++;
		if (buff[j] == '=') {
			token.type = SELFDIV;
		}
		else if (buff[j] == '/') {
			buffSize = 0;
			return nextToken();
		}
		else if (buff[j] == '*') {
			j += 1;
			i += 2;
			buffSize -= 2;
			curPos += 2;
			while (1) {
				token = nextToken();
				if (token.type == EXITCOMMEN) break;
			}
			return nextToken();
		}
		else {
			j--;
			token.type = DIV;
		}
		token.attr = 0;
	}
	else if (buff[j] == '=') {
		j++;
		if (buff[j] == '=') {
			token.type = EQU;
		}
		else {
			j--;
			token.type = ASS;
		}
		token.attr = 0;
	}
	else if (buff[j] == '<') {
		j++;
		if (buff[j] == '=') {
			token.type = LESSE;
		}
		else {
			j--;
			token.type = LESS;
		}
		token.attr = 0;
	}
	else if (buff[j] == '>') {
		j++;
		if (buff[j] == '=') {
			token.type = GREATE;
		}
		else {
			j--;
			token.type = GREAT;
		}
		token.attr = 0;
	}
	else if (buff[j] == '!') {
		j++;
		if (buff[j] == '=') {
			token.type = NEQU;
		}
		else {
			j--;
			token.type = ERR;
		}
		token.attr = 0;
	}
	else if (buff[j] == '(')
	{
		token.type = BRACKEO;
		token.attr = 0;
	}
	else if (buff[j] == ')')
	{
		token.type = BRACKEC;
		token.attr = 0;
	}
	else if (buff[j] == ',') {
		token.type = COMMA;
		token.attr = 0;
	}
	else if (buff[j] == '{') {
		token.type = BRACEO;
		token.attr = 0;
	}
	else if (buff[j] == '}') {
		token.type = BRACEC;
		token.attr = 0;
	}
	else if (buff[j] == '[') {
		token.type = FANGO;
		token.attr = 0;
	}
	else if (buff[j] == ']') {
		token.type = FANGC;
		token.attr = 0;
	}
	else if (buff[j] == ';') {
		token.type = SEMICOLON;
		token.attr = 0;
	}
	else
	{
		token.type = ERR;
	}
	token.r = curRow;
	token.c = i+1;
	buffSize -= (j - i + 1);
	i = j + 1;
	j = i;
	curPos = i;
	return token;
}void error(Token token) {
	cout << "行 " << token.r << "列 " << token.c;
	cout << "语法错误" << endl;
	//exit(0);
}


