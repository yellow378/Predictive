#include "predictive.h"
#include "lex.h"


Token curToken;
char buff[1024];																	//������
int buffSize = 0;
int curPos = 0;																		//��ǰλ��
int curRow = 0;
char textPath[100];																	//�������ļ�·��
char outPath[100] = "null";															//����ļ�·��
char keyTable[20][20] = { "if", "else", "int", "while", "do","****", "double", "void", "return" };	//�ؼ��ֱ�
char idTable[100][10];																//��ʶ����
int idsize = 0;																		//���ű������
char numTable[100][10];																//������
int numsize = 0;																	//�����������
bool fileOut = false;																//�Ƿ�������ļ�
FILE* of, * f;																		//��������ļ�

set<string> nonterminal, terminal;
string start;
map<string, set<string>> follow;
map<string, set<string>> first;
map<string, map<string, int>> predict;
int main() {
	terminal.insert("$");

	string buff;
	buff = getStringFromText("products.txt");
	vector<product> pros;

	//����ʼ����ʽ
	getProduct(buff,pros);
	cout << "��ʼ����ʽ" << endl;
	printProducts(pros);

	start = pros[0].left;

	//����ֱ����ݹ�
	pros = removeDirectLeftRecursion(pros);
	cout << "����ֱ����ݹ�" << endl;
	printProducts(pros);

	//TODO ���������ݹ�

	//��ȡ������
	//pros = unionLeftFactor(pros);
	//cout << "��ȡ������" << endl;
	//printProducts(pros);
	 
	//����First��
	getFirst(pros);
	cout << "����First��" << endl;
	printFirst(pros);
	
	//����Follow��
	getFollow(pros,follow);
	cout << "����Follow��" << endl;
	printFollow(follow);

	//�ж��Ƿ�ΪLL(1)�ķ�
	bool isll1 = isLL1(pros,follow);
	
	predict = getPredict(pros,follow);
	printPredict(predict);
	//����Ԥ�������
	if (isll1) {
		cout << "����LL1�ķ�\n" << endl;
		analyseProgram(pros,predict,follow,"program.txt");
	}
	else {
		cout << "������LL1�ķ�\n" << endl;
	}
	return 0;
}

