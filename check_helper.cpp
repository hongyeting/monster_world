#include<iostream>
#include<fstream>
#include<string>

using namespace std;

ifstream cin1("D:\\VS\\Warcraft.out");//标答
ifstream cin2("D:\\VS\\cindata.txt");//WA
ofstream cout0("D:\\VS\\coutdata.txt");

int id[2] = { 0 };
string s[2][10000000];

int main()
{
	for (int i = 0;i < 27;i++) {
		cout << i + 1;
		id[0] = id[1] = 0;
		getline(cin1, s[0][0]);
		getline(cin2, s[1][0]);
		while (cin1.peek() != '\n' && cin1.peek() != 'C')
			getline(cin1, s[0][++id[0]]);
		bool wrong = false;
		while (cin2.peek() != '\n' && cin2.peek() != 'C') {
			getline(cin2, s[1][++id[1]]);
			if (!wrong && (id[1] > id[0] || s[1][id[1]] != s[0][id[1]]))
				wrong = true;
		}
		if (id[1] < id[0])wrong = true;
		if (wrong) {
			int wrong = 0, j = 0;
			for (;j < id[0] && j < id[1];j++) {
				if (s[0][j] == s[1][j])cout0 << s[0][j] << '\n';
				else cout0 << '\n' << "wrong " << ++wrong << '\n' << s[0][j] << '\n' << s[1][j] << '\n';
			}
			for (;j < id[0];j++)
				cout0 << "default " << ++wrong << '\n' << s[0][j] << '\n';
			for (;j < id[1];j++)
				cout0 << "surplus" << ++wrong << '\n' << s[1][j] << '\n';
			cout0 << '\n' << '\n';
			cout << "wrong" << '\n';
		}
		cout << "correct" << '\n';
	}
	return 0;
}
