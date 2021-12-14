#include "predictive.h"

extern set<string> nonterminal, terminal;
extern string start;
extern map<string, set<string>> first;
//从文件获取全部信息
string getStringFromText(string path) {
	ifstream fin(path);
	stringstream buffer;            // stringstream object
	buffer << fin.rdbuf();          // read file content in stringstream object
	string str(buffer.str());       // store file content in a string
	return str;
}
//获取终结符和非终结符，将所有产生式保存到pros中
void getProduct(string buff,vector<product>& pros) {
	vector<string> words;
	getWords(buff, words);
	//获取非终结符
	for (int i = 0; i < words.size(); i++) {
		if (words[i] == "::=") {
			nonterminal.insert(words[i-1]);
		}
	}
	//获取终结符
	for (int i = 0; i < words.size(); i++) {
		if (words[i] != "::=" && nonterminal.find(words[i]) == nonterminal.end()) {
			terminal.insert(words[i]);
		}
	}
	//for (auto x : nonterminal) {
	//	cout << x << endl;
	//}
	//cout << "  ------  " << endl;
	//for (auto x : terminal) {
	//	cout << x << endl;
	//}
	
	//获取所有product
	product temp;
	for (int i = 0; i < words.size(); i++) {
		temp.right.clear();
		if (words[i] == "::=") {
			temp.left = words[i - 1];
			i++;
			while (i < words.size()) {
				if (i + 1 >= words.size() || words[i + 1] != "::=") {
					temp.right.push_back(words[i]);
					i++;
				}
				else {
					break;
				}
			}
			pros.push_back(temp);
		}
	}
}
//将字符串转换成字符串数组
void getWords(string buff,vector<string>& words) {
	istringstream iss(buff);
	string a;
	while (iss >> a) {
		words.push_back(a);
	}
}
//消除直接左递归
vector<product> removeDirectLeftRecursion(vector<product> pros) {
	int vis[1000];
	memset(vis, 0, sizeof(vis));
	vector<product> newpros;
	//int index = 1;
	for (int i = 0; i < pros.size(); i++) {
		//找到直接左递归的产生式
		if (pros[i].right.size() != 0 && pros[i].left == pros[i].right[0]) {
			string non = pros[i].left;
			product temp,a;
			a.left = non;
			temp.left = non + '1';//to_string(index);
			//找到所有对应的非左递归的产生式
			for (int j = 0; j < pros.size(); j++) {
				if (pros[j].left == pros[i].left && pros[j].right[0] != non) {
					if (vis[j] == 0) {
						for (int k = 0; k < pros[j].right.size(); k++) {
							a.right.push_back(pros[j].right[k]);
						}
						a.right.push_back(temp.left);
						newpros.push_back(a);
						vis[j] = 1;
					}
					for (int k = 1; k < pros[i].right.size(); k++) {
						temp.right.push_back(pros[i].right[k]);
					}
					temp.right.push_back(temp.left);
					newpros.push_back(temp);
				}
			}
			if (nonterminal.find(temp.left) == nonterminal.end()) {
				temp.right.clear();
				newpros.push_back(temp);
				nonterminal.insert(temp.left);
			}
		}
		else if(vis[i] == 0){
			newpros.push_back(pros[i]);
		}
	}
	return newpros;
}
void printProducts(vector<product> pros) {
	for (int i = 0; i < pros.size(); i++) {
		cout << pros[i].left << " ::= ";
		for (int j = 0; j < pros[i].right.size(); j++) {
			cout << pros[i].right[j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}
void getFirst(vector<product>& pros) {

	for(auto t = nonterminal.begin(); t != nonterminal.end(); t++)
		first[*t].clear();
	for (int i = 0; i < pros.size(); i++) {
		set<string> f = getOneFirst(pros,i);
		for(auto t = f.begin(); t != f.end(); t++)
			first[pros[i].left].insert(*t);
	}
}
set<string> getOneFirst(vector<product>& pros, int index) {
	product temp = pros[index];
	if (temp.right.size() == 0) {
		temp.first.insert("#");
	}
	else {
		int flagAll = 0;
		for (int i = 0; i < temp.right.size(); i++) {
			int flag = 0;
			if (nonterminal.find(temp.right[i]) != nonterminal.end()) {
				set<string> t1;
				for (int j = 0; j < pros.size(); j++) {
					if (pros[j].left == temp.right[i]) {
						set<string> t2 = getOneFirst(pros, j);
						set_union(t1.begin(),t1.end(),t2.begin(),t2.end(), inserter(t1, t1.begin()));
					}
				}
				for (auto j = t1.begin(); j != t1.end(); j++) {
					if (*j == "#") {
						flag = 1;
					}
					else {
						temp.first.insert(*j);
					}
				}
			}
			else {
				temp.first.insert(temp.right[i]);
			}
			if (i == temp.right.size() - 1 && flag == 1) {
				flagAll = 1;
			}
			if (flag == 0) {
				break;
			}
		}
		if (flagAll) {
			temp.first.insert("#");
		}
	}
	pros[index].first = temp.first;
	return temp.first;
}

void printFirst(vector<product> pros) {
	for (int i = 0; i < pros.size(); i++) {
		cout << "FIRST( "<<pros[i].left<< " ::= "; 
		for (auto x = pros[i].right.begin(); x != pros[i].right.end(); x++) {
			cout << " " << *x;
		}
		cout << " )=\t{";
		for (auto j = pros[i].first.begin(); j != pros[i].first.end(); j++) {
			cout << " " << *j;
		}
		cout << " }\n";
	}
	cout << endl<<endl;
	for (auto x = first.begin(); x != first.end(); x++) {
		cout << "FIRST(" << (*x).first << ")=\t{";
		for (auto y = (*x).second.begin(); y != (*x).second.end(); y++) {
			cout << " " << (*y);
		}
		cout << " }\n";
	}
	cout << endl;
}
void getFollow(vector<product> pros,map<string,set<string>>& follow) {
	for (auto s = nonterminal.begin(); s != nonterminal.end(); s++) {
		follow[*s].clear();
	}
	follow[start].insert("$");
	for (int it = 0; it < pros.size(); it++) {
		string left = pros[it].left;
		vector<string> right = pros[it].right;
		for (auto i = right.begin(); i != right.end(); i++) {
			if (terminal.find(*i) != terminal.end()) {
				continue;
			}
			else if((i+1) != right.end()){

				string word = *(i + 1);
				if (terminal.find(word) != terminal.end()) {
					follow[*i].insert(word);
				}
				else {
					for(auto x = first[word].begin(); x != first[word].end(); x++)
						if (*x != "#") {
							follow[*i].insert(*x);
						}
					}
				}
			}
	}
	while (getFollowBack(pros, follow)) {};
}
bool getFollowBack(vector<product> pros, map<string, set<string>>& follow) {
	bool flag = false;
	for (int i = 0; i < pros.size(); i++) {
		string left = pros[i].left;
		vector<string> right = pros[i].right;
		for (int j = right.size() - 1; j >= 0; j--) {
			if (nonterminal.find(right[j]) != nonterminal.end()) {
				for (auto it1 = follow[left].begin(); it1 != follow[left].end();it1++) {
					if (follow[right[j]].find(*it1) == follow[right[j]].end()) {
						flag = true;
						follow[right[j]].insert(*it1);
					}
				}
			}	
			else {
				break;
			}
			if (first[right[j]].find("#") == first[right[j]].end()) {
				break;
			}
		}
	}
	return flag;
}
void printFollow(map<string, set<string>> follow) {
	for (auto x = follow.begin(); x != follow.end(); x++) {
		cout << "FOLLOW(" << (*x).first << ")=\t{";
		for (auto y = (*x).second.begin(); y != (*x).second.end(); y++) {
			cout << " " << (*y);
		}
		cout << " }\n";
	}
	cout << endl;
}
bool isLL1(vector<product> pros,map<string,set<string>> follow) {
	//判断同一个产生式不同选择first集是否有交集
	for (int i = 0; i < pros.size(); i++) {
		string left1 = pros[i].left;
		set<string> f1 = pros[i].first;
		for (int j = i + 1; j < pros.size(); j++) {
			if (pros[j].left == left1) {
				set<string> f2 = pros[j].first;
				for (auto x = f1.begin(); x != f1.end(); x++) {
					if (f2.find(*x) != f2.end()) {
						return false;
					}
				}
			}
		}
	}
	//判断first有空的情况下，first集和follow是否有交集
	for (int i = 0; i < pros.size(); i++) {
		string left1 = pros[i].left;
		if (pros[i].first.find("#") != pros[i].first.end()) {
			set<string> f = follow[left1];
			for (int j = 0; j < pros.size(); j++) {
				if (i == j) continue;
				if (pros[j].left == left1) {
					for (auto x = f.begin(); x != f.end(); x++) {
						if (pros[j].first.find(*x) != pros[j].first.end()) {
							return false;
						}
					}
				}
			}

		}
	}
	return true;
}
vector<product> unionLeftFactor(vector<product> pros) {
	vector<product> newpros;
	int vis[1000];
	memset(vis, 0, sizeof(vis));
	for (auto x = nonterminal.begin(); x != nonterminal.end(); x++) {
		string name = *x;
		int index = 2;
		for (int i = 0; i < pros.size(); i++) {
			if (pros[i].left == name && pros[i].right.size() >= 1) {
				if (vis[i] == 1) continue;
				bool flag = false;
				string right = pros[i].right[0];
				for (int j = i + 1; j < pros.size(); j++) {
					if (vis[j] == 0 && pros[j].left == name && pros[j].right.size() >= 1 && pros[j].right[0] == right) {
						vis[j] = 1;
						flag = true;
						if (vis[i] == 0) {
							product p = pros[i];
							p.right.clear();
							p.right.push_back(pros[j].right[0]);
							p.right.push_back(p.left + to_string(index));
							newpros.push_back(p);

							p.left = p.left + to_string(index);
							p.right.clear();
							for (int k = 1; k < pros[i].right.size(); k++) {
								p.right.push_back(pros[i].right[k]);
							}
							newpros.push_back(p);
							vis[i] = 1;
							nonterminal.insert(p.left);
						}
						product p;
						p.left = pros[i].left + to_string(index);
						p.right.clear();
						for (int k = 1; k < pros[j].right.size(); k++) {
							p.right.push_back(pros[j].right[k]);
						}
						newpros.push_back(p);
					}
				}
				index++;
				if (flag == false) {
					newpros.push_back(pros[i]);
				}
			}
			else if (vis[i] == 0 && pros[i].right.size() == 0) {
				newpros.push_back(pros[i]);
				vis[i] = 1;
			}
		}
	}
	return newpros;
}

map<string, map<string, int>> getPredict(vector<product> pros,map<string, set<string>> follow) {
	map<string, map<string, int>> predict;
	for (int i = 0; i < pros.size(); i++) {
		string left = pros[i].left;
		for (auto x = pros[i].first.begin(); x != pros[i].first.end(); x++) {
			predict[left][*x] = i;
		}
		if (pros[i].first.find("#") != pros[i].first.end()) {
			for (auto y = follow[left].begin(); y != follow[left].end(); y++) {
				for (int j = 0; j < pros.size(); j++) {
					if (pros[j].left == left && pros[j].right.size() == 0) {
						predict[left][*y] = j;
						break;
					}
				}

			}
		}
	}

	return predict;
}
void printPredict(map<string, map<string, int>> predict) {
	cout << "预测";
	for (auto x = terminal.begin(); x != terminal.end(); x++) {
		cout << "\t" << *x;
	}
	cout <<endl<< endl;
	for (auto x = nonterminal.begin(); x != nonterminal.end(); x++) {
		cout << (*x);
		for (auto y = terminal.begin(); y != terminal.end(); y++) {
			if (predict[*x].find(*y) == predict[*x].end()) {
				cout << "\t--";
			}
			else {
				cout << "\t" << predict[*x][*y];
			}
			
		}
		cout << endl;
	}
}
