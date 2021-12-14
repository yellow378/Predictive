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

	//读初始产生式
	getProduct(buff,pros);
	cout << "初始产生式" << endl;
	printProducts(pros);

	start = pros[0].left;

	//消除直接左递归
	pros = removeDirectLeftRecursion(pros);
	cout << "消除直接左递归" << endl;
	printProducts(pros);

	//TODO 消除间接左递归

	//提取左因子
	//pros = unionLeftFactor(pros);
	//cout << "提取左因子" << endl;
	//printProducts(pros);
	 
	//计算First集
	getFirst(pros);
	cout << "计算First集" << endl;
	printFirst(pros);
	
	//计算Follow集
	getFollow(pros,follow);
	cout << "计算Follow集" << endl;
	printFollow(follow);

	//判断是否为LL(1)文法
	bool isll1 = isLL1(pros,follow);
	
	predict = getPredict(pros,follow);
	printPredict(predict);
	//计算预测分析表
	if (isll1) {
		cout << "满足LL1文法\n" << endl;
	}
	else {
		cout << "不满足LL1文法\n" << endl;
	}
	return 0;
}

