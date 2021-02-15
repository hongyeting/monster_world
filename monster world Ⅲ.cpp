//魔兽世界之三：开战

#include<iostream>
#include<cstring>
#include<iomanip>
#include<vector>

using namespace std;

int N = 0, K = 0, T = 0, timee = 0;
/*
	两个司令部之间一共有N个城市(1 <= N <= 20)
	lion每前进一步，忠诚度就降低K(0 <= K <= 100)
	T以分钟为单位(0 <= T <= 6000)
*/

int a[5] = { 0 };//制造武士所需的生命元
int b[5] = { 0 };//武士的初始攻击力
int c[2][5] = { {2,3,4,1,0},{3,0,1,2,4} };//制造武士的顺序
int cid[2] = { 4,4 };//上次制造哪里了
string warrior_name[5] = { "dragon","ninja","iceman","lion","wolf" };//武士的名字
string weapon_name[3] = { "sword","bomb","arrow" };
string color_name[2] = { "red","blue" };
int life[2];//司令部的生命元
int city[22][2] = { 0 };//city[N+1]为蓝方司令部

struct warriors
{
	int attribute;//属性
	/*
		0-dragon
		1-ninja
		2-iceman
		3-lion
		4-wolf
	*/
	int element;//生命元
	int force;//攻击力（似乎不会变化？
	int camp;//阵营color
	int id;//序号
	int place;//城市位置0-N+1
	int sum;//武器总数（至多10件
	int weapon[3];//三种武器的数量
	int arrow;//用过一次的箭的数量
	int u, v;//正在/上次使用的武器名为u，是该种武器的第v件
	int loyalty;//狮子的忠诚度
};

warriors warrior[2][10000];

void coutime()
{
	int t = timee / 60;int dgt = 0;
	while (t) { dgt++;t = t / 10; }
	while (3 - dgt > 0) { cout << 0;dgt++; }
	if (timee / 60)cout << timee / 60;
	cout << ":";
	t = timee % 60;
	if (t / 10 == 0)cout << 0;
	cout << t;
}

bool weak(warriors* warrior)//判断是否存在攻击力为0的情况，若存在则返回1
{
	if (warrior->weapon[0] > 0 && warrior->force / 5 == 0)return 1;//sword
	if (warrior->weapon[1] > 0 && (warrior->force * 2) / 5 == 0)return 1;//bomb
	if (warrior->weapon[2] > 0 && (warrior->force * 3) / 10 == 0)return 1;//arrow
	return 0;
}

void fight(warriors* gong, warriors* shou)//某战士攻击对手一次（或无法攻击）
{
	if (gong->sum > 0)
	{
		while (gong->weapon[gong->u] <= 0)
		{
			gong->u = (gong->u + 1) % 3;
			gong->v = 0;//为了适用继续使用同一种武器的代码
		}
		switch (gong->u)
		{
		case 0://剑
			shou->element -= gong->force / 5;
			break;
		case 1://炸弹
			gong->weapon[1]--;gong->sum--;gong->v--;//补上没了的
			shou->element -= (gong->force * 2) / 5;
			if (gong->attribute != 1)
				gong->element -= ((gong->force * 2) / 5) / 2;
			break;
		case 2://箭
			if (gong->arrow > 0)//用已经用过的箭
			{
				shou->element -= (gong->force * 3) / 10;
				gong->weapon[2]--;gong->arrow--;gong->sum--;
				gong->v--;//补上没了的
			}
			else//用还没用过的箭
			{
				shou->element -= (gong->force * 3) / 10;
				gong->arrow++;
			}
			break;
		default:
			break;
		}
		gong->v++;//这次用的编号（已补上没了的
		if (gong->v == gong->weapon[gong->u])
		{
			gong->u = (gong->u + 1) % 3;
			gong->v = 0;
		}
	}
	return;//死没死不一定
}

bool steal(warriors* gong, warriors* shou, bool legitimate)//抢到了返回1
{
	/*
		wolf抢武器
		000:35 blue wolf 2 took 3 bomb from red dragon 2 in city 4
	*/
	if (gong->sum == 10 || shou->sum == 0)return 0;
	int target = 0;
	while (target < 3 && shou->weapon[target] == 0)target++;
	if (shou->weapon[target] + gong->sum <= 10)//全部抢来
	{
		if (!legitimate)
		{
			coutime();cout << " " << color_name[gong->camp] << " wolf " << gong->id << " took ";
			cout << shou->weapon[target] << " " << weapon_name[target] << " from " << color_name[shou->camp];
			cout << " " << warrior_name[shou->attribute] << " " << shou->id << " in city " << gong->place << '\n';
		}
		gong->sum += shou->weapon[target];
		gong->weapon[target] += shou->weapon[target];
		shou->sum -= shou->weapon[target];
		shou->weapon[target] = 0;
		if (target == 2)//抢的是箭
		{
			gong->arrow += shou->arrow;
			shou->arrow = 0;
		}
	}
	else
	{
		if (target == 2)//抢的是箭
		{
			if ((10 - gong->sum) > (shou->weapon[2] - shou->arrow))//抢了用过一次的箭
			{
				gong->arrow += (10 - gong->sum + shou->arrow - shou->weapon[2]);
				shou->arrow = shou->weapon[2] - 10 + gong->sum;
			}
		}
		if (!legitimate)
		{
			coutime();cout << " " << color_name[gong->camp] << " wolf " << gong->id << " took ";
			cout << 10 - gong->sum << " " << weapon_name[target] << " from " << color_name[shou->camp];
			cout << " " << warrior_name[shou->attribute] << " " << shou->id << " in city " << gong->place << '\n';
		}
		gong->weapon[target] += (10 - gong->sum);
		shou->sum -= (10 - gong->sum);
		shou->weapon[target] -= (10 - gong->sum);
		gong->sum = 10;
	}
	return 1;
}

int main()
{
	int NM = 0;cin >> NM;
	for (int nm = 1;nm <= NM;nm++)
	{
		cin >> life[0] >> N >> K >> T;
		timee = 0;life[1] = life[0];
		cid[1] = cid[0] = 4;
		memset(city, 0, sizeof(city));
		memset(warrior, 0, sizeof(warrior));
		for (int i = 0;i < 5;i++)cin >> a[i];//生命值
		for (int i = 0;i < 5;i++)cin >> b[i];//攻击力
		int number = 0;//新建武士的序号
		int finalsum[2] = { 0 };//可能不再有新的武士诞生，编号停在哪里
		bool cok[2] = { true,true };//是否仍然可以新建武士
		cout << "Case " << nm << ":" << '\n';
		while (true)
		{
			number++;
			/*
				武士降生
				000:00 blue lion 1 born
				Its loyalty is 24
			*/
			for (int clr = 0;clr < 2;clr++)
			{
				if (cok[clr])
				{
					cid[clr] = (cid[clr] + 1) % 5;//制造顺序指示
					int p = c[clr][cid[clr]];//武士属性attribute
					if (a[p] > life[clr])//停止制造武士
						cok[clr] = 0;
					else
					{
						finalsum[clr]++;//某方武士总数
						life[clr] -= a[p];
						warrior[clr][number].attribute = p;
						if (p != 4)//not wolf
						{
							warrior[clr][number].weapon[number % 3] = 1;
							warrior[clr][number].sum++;
						}
						if (p == 1)//ninja
						{
							warrior[clr][number].weapon[(number + 1) % 3] = 1;
							warrior[clr][number].sum++;
						}
						warrior[clr][number].attribute = p;//attribute属性
						warrior[clr][number].element = a[p];//element生命值
						warrior[clr][number].force = b[p];//force攻击力
						warrior[clr][number].camp = clr;//camp阵营
						warrior[clr][number].id = number;//id序列号
						switch (clr)//place位置/修改city数组
						{
						case 0:warrior[clr][number].place = 0;city[0][0] = number;break;
						default:warrior[clr][number].place = N + 1;city[N + 1][1] = number;break;
						}
						coutime();cout << " " << color_name[clr] << " " << warrior_name[p] << " " << number << " born" << '\n';
						if (p == 3)//lion
						{
							warrior[clr][number].loyalty = life[clr];//loyalty忠诚度
							cout << "Its loyalty is " << life[clr] << '\n';
						}
					}
				}
			}
			timee += 5;
			if (timee > T)break;
			/*
				lion逃跑
				000:05 blue lion 1 ran away
			*/
			for (int i = 1;i <= N;i++)
			{
				for (int clr = 0;clr < 2;clr++)
				{
					if (city[i][clr] != 0)
					{
						if (warrior[clr][city[i][clr]].attribute == 3)
							if (warrior[clr][city[i][clr]].loyalty <= 0)//跑路了！
							{
								coutime();cout << " " << color_name[clr] << " lion " << city[i][clr] << " ran away" << '\n';
								warrior[clr][city[i][clr]].element = 0;//社死，便于后续操作
								city[i][clr] = 0;
							}
					}
				}
			}
			timee += 5;
			if (timee > T)break;
			/*
				武士行进
				000:10 red iceman 1 marched to city 1 with 20 elements and force 30
				武士抵达敌军司令部
				001:10 red iceman 1 reached blue headquarter with 20 elements and force 30
				司令部被占领
				003:10 blue headquarter was taken
			*/
			bool end = 0;
			if (city[1][1] > 0)
			{
				if (warrior[1][city[1][1]].attribute == 2)//iceman
					warrior[1][city[1][1]].element -= warrior[1][city[1][1]].element / 10;
				coutime();cout << " blue " << warrior_name[warrior[1][city[1][1]].attribute] << " " << city[1][1] << " reached red headquarter with ";
				cout << warrior[1][city[1][1]].element << " elements and force " << warrior[1][city[1][1]].force << '\n';//蓝方抵达红方司令部
				coutime();cout << " red headquarter was taken" << '\n';//红方司令部被占领
				end = 1;
			}
			for (int cty = 1;cty <= N;cty++)
			{
				if (city[cty - 1][0] != 0)
				{
					if (warrior[0][city[cty - 1][0]].attribute == 2)//iceman
						warrior[0][city[cty - 1][0]].element -= (warrior[0][city[cty - 1][0]].element / 10);
					else if (warrior[0][city[cty - 1][0]].attribute == 3)//lion
						warrior[0][city[cty - 1][0]].loyalty -= K;
					if (warrior[0][city[cty - 1][0]].element > 0)
					{
						coutime();cout << " red " << warrior_name[warrior[0][city[cty - 1][0]].attribute] << " " << city[cty - 1][0] << " marched to city " << cty << " with ";
						cout << warrior[0][city[cty - 1][0]].element << " elements and force " << warrior[0][city[cty - 1][0]].force << '\n';
					}
				}
				if (city[cty + 1][1] != 0)
				{
					if (warrior[1][city[cty + 1][1]].attribute == 2)//iceman
						warrior[1][city[cty + 1][1]].element -= (warrior[1][city[cty + 1][1]].element / 10);
					else if (warrior[1][city[cty + 1][1]].attribute == 3)//lion
						warrior[1][city[cty + 1][1]].loyalty -= K;
					if (warrior[1][city[cty + 1][1]].element > 0)
					{
						coutime();cout << " blue " << warrior_name[warrior[1][city[cty + 1][1]].attribute] << " " << city[cty + 1][1] << " marched to city " << cty << " with ";
						cout << warrior[1][city[cty + 1][1]].element << " elements and force " << warrior[1][city[cty + 1][1]].force << '\n';
					}
				}
			}
			if (city[N][0] > 0)
			{
				if (warrior[0][city[N][0]].attribute == 2)//iceman
					warrior[0][city[N][0]].element -= warrior[0][city[N][0]].element / 10;
				coutime();cout << " red " << warrior_name[warrior[0][city[N][0]].attribute] << " " << city[N][0] << " reached blue headquarter with ";
				cout << warrior[0][city[N][0]].element << " elements and force " << warrior[0][city[N][0]].force << '\n';//红方抵达蓝方司令部
				coutime();cout << " blue headquarter was taken" << '\n';//蓝方司令部被占领
				end = 1;
			}
			if (end)break;
			//修改city数组和武士位置
			for (int clr = 0;clr < 2;clr++)
			{
				for (int i = 1;i <= finalsum[clr];i++)
				{
					if (warrior[clr][i].element > 0)
					{
						int move[2] = { 1,-1 };
						if ((warrior[clr][i].place >= 1 && warrior[clr][i].place <= N) || (warrior[clr][i].place == 0 && clr == 0) || (warrior[clr][i].place == N + 1 && clr == 1))
						{
							city[warrior[clr][i].place][clr] = 0;
							warrior[clr][i].place += move[clr];
							city[warrior[clr][i].place][clr] = i;
						}
					}
				}
			}
			timee += 25;
			if (timee > T)break;
			/*
				wolf抢武器
				000:35 blue wolf 2 took 3 bomb from red dragon 2 in city 4
			*/
			int war_city[21] = { 0 };//发生战争的城市
			int war_num = 0;
			for (int i = 1;i <= N;i++)
			{
				if (city[i][0] > 0 && city[i][1] > 0)
				{
					war_city[war_num] = i;war_num++;
				}
			}
			for (int i = 0;i < war_num;i++)
			{
				warriors* warrior1 = &warrior[0][city[war_city[i]][0]], * warrior2 = &warrior[1][city[war_city[i]][1]];
				if (warrior1->attribute == 4 && warrior2->attribute != 4)
					steal(warrior1, warrior2, false);
				else if (warrior1->attribute != 4 && warrior2->attribute == 4)
					steal(warrior2, warrior1, false);
			}
			timee += 5;
			if (timee > T)break;
			/*
				组织打架;
				报告战果
				000:40 red iceman 1 killed blue lion 12 in city 2 remaining 20 elements
				000:40 both red iceman 1 and blue lion 12 died in city 2
				000:40 both red iceman 1 and blue lion 12 were alive in city 2
				武士欢呼
				003:40 blue dragon 2 yelled in city 4
			*/
			for (int i = 0;i < war_num;i++)
			{
				bool gong = 0;
				int situation = 0;//1-单死/2-都死/3-都活
				if (war_city[i] % 2)gong = 1;//奇数城市红方先进攻
				warriors* warrior1 = &warrior[0][city[war_city[i]][0]], * warrior2 = &warrior[1][city[war_city[i]][1]];
				warrior1->u = warrior1->v = warrior2->u = warrior2->v = 0;
				if (weak(warrior1) || weak(warrior2))//存在假的武器，麻烦一点儿
				{
					int weapon_max = warrior1->sum > warrior2->sum ? warrior1->sum : warrior2->sum;//一轮武器最大有多少
					weapon_max *= 4;//双方，∴*2；箭用两次，∴再*2。
					while (true)//直到至少一方死亡或者双方都没有武器
					{
						int element1 = warrior1->element, element2 = warrior2->element;
						for (int j = 1;j <= weapon_max;j++)
						{
							if (warrior1->element <= 0 && warrior2->element <= 0)
							{
								situation = 2;break;
							}
							if (warrior1->element <= 0 || warrior2->element <= 0)
							{
								situation = 1;break;
							}
							if (warrior1->sum == 0 && warrior2->sum == 0)
							{
								situation = 3;break;
							}
							if (gong)fight(warrior1, warrior2);
							else fight(warrior2, warrior1);
							gong = !gong;
						}
						if (situation != 0)break;
						else if (element1 == warrior1->element && element2 == warrior2->element)
						{
							situation = 3;break;
						}
					}
				}
				else
				{
					while (true)//直到至少一方死亡或者双方都没有武器
					{
						if (warrior1->element <= 0 && warrior2->element <= 0)
						{
							situation = 2;break;
						}
						if (warrior1->element <= 0 || warrior2->element <= 0)
						{
							situation = 1;break;
						}
						if (warrior1->sum == 0 && warrior2->sum == 0)
						{
							situation = 3;break;
						}
						if (gong)fight(warrior1, warrior2);
						else fight(warrior2, warrior1);
						gong = !gong;
					}
				}
				switch (situation)
				{
				case 1://单死
				{
					warriors* winner = warrior1->element > 0 ? warrior1 : warrior2;
					warriors* loser = warrior1->element <= 0 ? warrior1 : warrior2;
					coutime();cout << " " << color_name[winner->camp] << " " << warrior_name[winner->attribute] << " " << winner->id << " killed ";
					cout << color_name[loser->camp] << " " << warrior_name[loser->attribute] << " " << loser->id << " in city " << war_city[i];
					cout << " remaining " << winner->element << " elements" << '\n';
					if (winner->attribute == 0)//dragon
					{
						coutime();cout << " " << color_name[winner->camp] << " dragon " << winner->id << " yelled in city " << war_city[i] << '\n';
					}
					while (steal(winner, loser, true));//winner缴获武器
					break;
				}
				case 2://双死
					coutime();cout << " both red " << warrior_name[warrior1->attribute] << " " << warrior1->id << " and blue ";
					cout << warrior_name[warrior2->attribute] << " " << warrior2->id << " died in city " << war_city[i] << '\n';
					break;
				case 3://双活
					coutime();cout << " both red " << warrior_name[warrior1->attribute] << " " << warrior1->id << " and blue ";
					cout << warrior_name[warrior2->attribute] << " " << warrior2->id << " were alive in city " << war_city[i] << '\n';
					if (warrior1->attribute == 0)//dragon
					{
						coutime();cout << " " << color_name[warrior1->camp] << " dragon " << warrior1->id << " yelled in city " << war_city[i] << '\n';
					}
					if (warrior2->attribute == 0)//dragon
					{
						coutime();cout << " " << color_name[warrior2->camp] << " dragon " << warrior2->id << " yelled in city " << war_city[i] << '\n';
					}
					break;
				default:
					cout << "00000000000";//不会吧
					break;
				}
			}
			//修改city数组
			for (int i = 0;i < war_num;i++)
			{
				for (int clr = 0;clr < 2;clr++)
				{
					if (warrior[clr][city[war_city[i]][clr]].element <= 0)
						city[war_city[i]][clr] = 0;
				}
			}
			timee += 10;
			if (timee > T)break;
			/*
				司令部报告
				000:50 100 elements in red headquarter
				000:50 120 elements in blue headquarter
			*/
			for (int clr = 0;clr < 2;clr++)
			{
				coutime();cout << " " << life[clr] << " elements in " << color_name[clr] << " headquarter" << '\n';
			}
			timee += 5;
			if (timee > T)break;
			/*
				武士报告情况
				000:55 blue wolf 2 has 2 sword 3 bomb 0 arrow and 7 elements
			*/
			for (int i = 1;i <= N;i++)
			{
				for (int clr = 0;clr < 2;clr++)
				{
					if (city[i][clr] != 0)
					{
						coutime();cout << " " << color_name[clr] << " " << warrior_name[warrior[clr][city[i][clr]].attribute] << " " << warrior[clr][city[i][clr]].id << " has ";
						for (int w = 0;w < 3;w++)
							cout << warrior[clr][city[i][clr]].weapon[w] << " " << weapon_name[w] << " ";
						cout << "and " << warrior[clr][city[i][clr]].element << " elements" << '\n';
					}
				}
			}
			timee += 5;
			if (timee > T)break;
		}
	}
	return 0;
}
