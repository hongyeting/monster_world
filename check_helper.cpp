#include<iostream>
#include<fstream>
#include<string>

using namespace std;

ifstream cin1("D:\\VS\\Warcraft.out");//标答
ifstream cin2("D:\\VS\\cindata.txt");//WA
ofstream cout0("D:\\VS\\coutdata.txt");

string s[3];

int main()
{
	int x = 0;
	int w = 0;
	while (cin1.peek() != '\n' && cin2.peek() != '\n' && w < 50)
	{
		x++;
		getline(cin1, s[0]);
		getline(cin2, s[1]);
		if (s[0][0] == 'C')
			s[2] = s[0];
		if (s[0] != s[1])
		{
			w++;
			if (w == 1)cout << s[2] << '\n' << '\n';
			cout0 << s[0] << '\n' << s[1] << '\n' << "wrong " << w << '\n' << '\n';
		}
	}
	cout << w << '\n';
	return 0;
}
