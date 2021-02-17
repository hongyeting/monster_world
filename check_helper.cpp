#include<fstream>
#include<string>

using namespace std;

ifstream cin("D:\\VS\\cindata.txt");
ofstream cout("D:\\VS\\coutdata.txt");

string s[10000];

int main()
{
	int x = 0;//x lines
	for (;;x++)
	{
		getline(cin, s[x]);cout << 1;
		if (s[x] == "abcdefg" || x == 9999)break;
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
