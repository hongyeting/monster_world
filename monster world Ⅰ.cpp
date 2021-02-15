//魔兽世界之一：备战

#include<iostream>
#include<cstring>

using namespace std;

int a[2][5] = { 0 };//制造武士所需的生命元
string name[2][5] = { {"iceman","lion","wolf","ninja","dragon"},{"lion","dragon","ninja","iceman","wolf"} };//武士的名字
int worrier_id[2][5] = { 0 };//每种武士的总数
int worrier_num[2] = { 0 };//上一次诞生的武士
int life[2] = { 0 };//生命元的余额

bool valor(int clr, int time)
{
	if (life[clr] == -1)return 0;
	for (int i = 1;i <= 5;i++)
	{
		worrier_num[clr] = (worrier_num[clr] + 1) % 5;
		if (a[clr][worrier_num[clr]] <= life[clr])//000 red iceman 1 born with strength 5,1 iceman in red headquarter
		{
			int t = time;int dgt = 0;
			while (t) { dgt++;t = t / 10; }
			while (3 - dgt > 0) { cout << "0";dgt++; }
			if (time)cout << time;
			if (clr == 0)cout << " red ";
			else cout << " blue ";
			cout << name[clr][worrier_num[clr]] << " " << time + 1 << " born with strength " << a[clr][worrier_num[clr]] << "," << ++worrier_id[clr][worrier_num[clr]] << " " << name[clr][worrier_num[clr]] << " in";
			if (clr == 0)cout << " red ";
			else cout << " blue ";
			cout << "headquarter" << '\n';
			life[clr] -= a[clr][worrier_num[clr]];
			return 1;
		}
	}
	//003 red headquarter stops making warriors
	int t = time;int dgt = 0;
	while (t) { dgt++;t = t / 10; }
	while (3 - dgt > 0) { cout << "0";dgt++; }
	if (time)cout << time;
	if (clr == 0)cout << " red ";
	else cout << " blue ";
	cout << "headquarter stops making warriors" << '\n';
	life[clr] = -1;return 0;
}

int main()
{
	int NM = 0;cin >> NM;
	for (int nm = 0;nm < NM;nm++)
	{
		cin >> life[0];life[1] = life[0];
		cin >> a[0][4] >> a[0][3] >> a[0][0] >> a[0][1] >> a[0][2];
		a[1][0] = a[0][1], a[1][1] = a[0][4], a[1][2] = a[0][3], a[1][3] = a[0][0], a[1][4] = a[0][2];
		worrier_num[1] = worrier_num[0] = 4;
		memset(worrier_id, 0, sizeof(worrier_id));
		cout << "Case:" << nm + 1 << '\n';
		for (int ti = 0;;ti++)
		{
			bool x = valor(0, ti), y = valor(1, ti);
			if (!x && !y)break;
		}
	}
	return 0;
}

