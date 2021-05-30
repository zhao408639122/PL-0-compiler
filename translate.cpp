#include<cstdio>
#include<iostream>
#include<string>
#include<vector>

using namespace std;
vector<string> tokens;
string ans;
void dfs(int &index, int depth)
{
again:
	ans += string(depth, '\t');
	ans += tokens[index];
	index++;
	if (index < tokens.size() && tokens[index] == "(")
	{
		ans += "(\n";
		index++;
		dfs(index, depth + 1);
		if (tokens[index] != ")")throw 233;
		ans += string(depth,'\t') +")";
		index++;
	}
	if (index < tokens.size() && tokens[index] == ",")
	{
		ans += ",\n";
		index++;
		goto again;
	}
	ans += "\n";
}
int main()
{
	string exp;
	cin >> exp;
	string tmp;
	for (int i = 0; i < exp.length(); i++)
	{
		if (exp[i] == '(' || exp[i] == ')' || exp[i] == ',')
		{
			if(!tmp.empty())tokens.push_back(tmp);
			tmp = "";
			tokens.push_back(string(1, exp[i]));
		}
		else tmp += exp[i];
	}
	if (!tmp.empty())tokens.push_back(tmp);
	try
	{
		int id = 0;
		dfs(id, 0);
	}
	catch (...)
	{
		cerr << "Error Parsing Expression" << endl;
		return -1;
	}
	cout << ans << endl;
}