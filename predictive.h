#pragma once
#include<iostream>
#include<stdio.h>
#include<fstream>
#include<sstream>
#include<cstdio>
#include<vector>
#include<String>
#include<set>
#include<map>
#include<algorithm>
#include<stack>
using namespace std;

typedef struct product {
	string left;
	vector<string> right;
	set<string> first;
	set<string> follow;
	bool operator < (product a) {
		return left < a.left;
	}
}product;

string getStringFromText(string paht);
void getProduct(string buff,vector<product>& pros);
void getWords(string buff, vector<string>& words);
vector<product> removeDirectLeftRecursion(vector<product> pros);
void printProducts(vector<product> pros);
void getFirst(vector<product>& pros);
set<string> getOneFirst(vector<product> & pros,int index);
void printFirst(vector<product> pros);
void getFollow(vector<product> pros,map<string,set<string>>& follow);
bool getFollowBack(vector<product> pros, map<string, set<string>>& follow);
void printFollow(map<string, set<string>> follow);
bool isLL1(vector<product> pros,map<string,set<string>> follow);
vector<product> unionLeftFactor(vector<product> pros);
void printPredict(map<string,map<string,int>> predict);
map<string, map<string, int>> getPredict(vector<product> pros, map<string, set<string>> follow);
bool analyseProgram(vector<product> pros, map<string,map<string,int>> predict,map<string,set<string>> follow,const char* path);
