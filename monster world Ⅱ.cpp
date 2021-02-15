//魔兽世界之二：装备

#include<iostream>
#include<cstring>
#include<iomanip>

using namespace std;

int a[2][5] = { 0 };//制造武士所需的生命元
string name[2][5] = { {"iceman","lion","wolf","ninja","dragon"},{"lion","dragon","ninja","iceman","wolf"} };//武士的名字
string weaponame[3] = { "sword","bomb","arrow" };
int weaponid[2][5] = { {1,2,3,4,5},{2,5,4,1,3} };
int warrier_id[2][5] = { 0 };//每种武士的总数
int warrier_num[2] = { 0 };//上一次诞生的武士
int life[2] = { 0 };//生命元的余额
int ti = 0;

void weapon(int id, int color)
{
	switch (id)
	{
	case 1:
		cout << "It has a " << weaponame[(ti + 1) % 3] << '\n';
		break;
	case 2:
		cout << "It's loyalty is " << life[color] << '\n';
		break;
	case 3:
		break;
	case 4:
		cout << "It has a " << weaponame[(ti + 1) % 3] << " and a " << weaponame[(ti + 2) % 3] << '\n';
		break;
	case 5:
		cout << "It has a " << weaponame[(ti + 1) % 3] << ",and it's morale is " << fixed << setprecision(2) << (double)life[color] / a[0][4] << '\n';
		break;
	default:break;
	}
}

bool valor(int clr, int time)
{
	if (life[clr] == -1)return 0;
	for (int i = 1;i <= 5;i++)
	{
		warrier_num[clr] = (warrier_num[clr] + 1) % 5;
		if (a[clr][warrier_num[clr]] <= life[clr])//000 red iceman 1 born with strength 5,1 iceman in red headquarter
		{
			int t = time;int dgt = 0;
			while (t) { dgt++;t = t / 10; }
			while (3 - dgt > 0) { cout << "0";dgt++; }
			if (time)cout << time;
			if (clr == 0)cout << " red ";
			else cout << " blue ";
			cout << name[clr][warrier_num[clr]] << " " << time + 1 << " born with strength " << a[clr][warrier_num[clr]] << "," << ++warrier_id[clr][warrier_num[clr]] << " " << name[clr][warrier_num[clr]] << " in";
			if (clr == 0)cout << " red ";
			else cout << " blue ";
			cout << "headquarter" << '\n';
			life[clr] -= a[clr][warrier_num[clr]];
			weapon(weaponid[clr][warrier_num[clr]], clr);
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
		warrier_num[1] = warrier_num[0] = 4;
		memset(warrier_id, 0, sizeof(warrier_id));
		cout << "Case:" << nm + 1 << '\n';
		for (ti = 0;;ti++)
		{
			bool x = valor(0, ti), y = valor(1, ti);
			if (!x && !y)break;
		}
	}
	return 0;
}
