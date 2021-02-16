#include<iostream>
#include<string>

using namespace std;

string s[10000];

int main()
{
	int x = 0;//x lines
	for (;;x++)
	{
		getline(cin, s[x]);
		if (s[x] == "abcdefg")break;
	}
	string ss;
	int wrong = 0;
	for (int i = 0;i < x;i++)
	{
		getline(cin, ss);
		if (ss == s[i])cout << "right" << i + 1 << '\n';
		else { wrong++;cout << s[i] << '\n' << '\n' << "wrong" << i + 1 << " " << wrong << '\n'; }
	}
	cout << wrong;
	return 0;
}
