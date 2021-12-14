#include "predictive.h"

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
	}
	else {
		cout << "������LL1�ķ�\n" << endl;
	}
	return 0;
}

