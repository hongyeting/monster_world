//魔兽世界之四：终极

#include<iostream>
#include<cstring>
#include<iomanip>
#include<vector>

using namespace std;

int N = 0, R = 0, K = 0, T = 0, timee = 0;
/*
	两个司令部之间一共有N个城市(1 <= N <= 20)
	arrow的攻击力(R)
	lion每前进一步，忠诚度就降低K(0 <= K <= 100)
	T以分钟为单位(0 <= T <= 6000)
*/

enum WARRIOR
{
	dragon, ninja, iceman, lion, wolf
};

enum WEAPON
{
	sword, bomb, arrow
};

enum COLOR
{
	red, blue
};

int a[5] = { 0 };//制造武士所需的生命元
int b[5] = { 0 };//武士的初始攻击力
int c[2][5] = { {2,3,4,1,0},{3,0,1,2,4} };//制造武士的顺序
int cid[2] = { 4,4 };//上次制造到哪里了
string warrior_name[5] = { "dragon","ninja","iceman","lion","wolf" };//武士的名字
string weapon_name[3] = { "sword","bomb","arrow" };
string color_name[2] = { "red","blue" };

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
	bool weapon[3];//三种武器的数量
	int arrow;//箭还能用几次
	int sword;//箭的攻击力
	int loyalty;//狮子的忠诚度
	double morale;//龙的士气
	bool walk;//雪怪是否偶数步
};

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

/*
	被攻击者生命值会减去进攻者的攻击力值和进攻者手中sword的攻击力值。
	被进攻者若没死，就会发起反击，被反击者的生命值要减去反击者攻击力值的一半(去尾取整)和反击者手中sword的攻击力值。
	sword每经过一次战斗(不论是主动攻击还是反击)，就会变钝，攻击力变为本次战斗前的80% (去尾取整)。
	sword攻击力变为0时，视为武士失去了sword。
*/
bool fight(warriors* gong, warriors* shou, bool re = false)//某战士攻击对手一次，杀死敌人返回1
{
	int f = re ? gong->force / 2 : gong->force;
	if (gong->weapon[sword])
	{
		f += gong->sword;
		gong->sword = gong->sword * 4 / 5;
		if (gong->sword == 0)
			gong->weapon[sword] = false;
	}
	shou->element -= f;
	if (shou->element <= 0)
		return 1;
	return 0;
}

int main()
{
	int NM = 0;cin >> NM;
	for (int nm = 1;nm <= NM;nm++)
	{
		int life[2];//司令部的生命元
		int city[22][2] = { 0 };//city[N+1]为蓝方司令部
		int city_history[22][4] = { 0 };
		/*
			city_history[i][0]为积累的生命元数量
			city_history[i][1、2]记录最近两次对局：0-无，1-red，2-blue，3-draw（其实记录一次就够了）
			city_history[i][3]为旗帜状态：0-无，1-red，2-blue
		*/
		warriors warrior[2][100];
		cin >> life[red] >> N >> R >> K >> T;
		timee = 0;life[blue] = life[red];
		cid[red] = cid[blue] = 4;
		for (int i = 0;i < 5;i++)cin >> a[i];//生命值
		for (int i = 0;i < 5;i++)cin >> b[i];//攻击力
		int number[2] = { 0 };//新建武士的序号
		int occupy[2] = { 0 };//2个敌方士兵就被攻陷了
		cout << "Case " << nm << ":" << '\n';
		while (true)
		{
			/*
				武士降生
				000:00 blue lion 1 born
				Its loyalty is 24
				000:00 blue dragon 1 born
				Its morale is 23.34
			*/
			for (int clr = 0;clr < 2;clr++)
			{
				cid[clr] = (cid[clr] + 1) % 5;//制造顺序指示
				int p = c[clr][cid[clr]];//武士属性attribute
				if (a[p] > life[clr])//不能制造武士
					cid[clr] = (cid[clr] + 4) % 5;
				else
				{
					number[clr]++;//某方武士总数
					life[clr] -= a[p];
					warrior[clr][number[clr]].attribute = p;
					for (int w = 0;w < 3;w++)warrior[clr][number[clr]].weapon[w] = false;
					if (p != wolf && p != lion)//wolf||lion没武器
						warrior[clr][number[clr]].weapon[number[clr] % 3] = true;
					if (p == ninja)//ninja
						warrior[clr][number[clr]].weapon[(number[clr] + 1) % 3] = true;
					if (warrior[clr][number[clr]].weapon[sword])//sword
					{
						int fierce = b[p] / 5;
						if (fierce == 0)
							warrior[clr][number[clr]].weapon[0] = false;
						else
							warrior[clr][number[clr]].sword = fierce;
					}
					if (warrior[clr][number[clr]].weapon[arrow])//arrow
						warrior[clr][number[clr]].arrow = 3;
					warrior[clr][number[clr]].attribute = p;//attribute属性
					warrior[clr][number[clr]].element = a[p];//element生命值
					warrior[clr][number[clr]].force = b[p];//force攻击力
					warrior[clr][number[clr]].camp = clr;//camp阵营
					warrior[clr][number[clr]].id = number[clr];//id序列号
					switch (clr)//place位置/修改city数组
					{
					case 0:warrior[clr][number[clr]].place = 0;city[0][0] = number[clr];break;
					default:warrior[clr][number[clr]].place = N + 1;city[N + 1][1] = number[clr];break;
					}
					coutime();cout << " " << color_name[clr] << " " << warrior_name[p] << " " << number[clr] << " born" << '\n';
					if (p == lion)//lion
					{
						warrior[clr][number[clr]].loyalty = life[clr];//loyalty忠诚度
						cout << "Its loyalty is " << life[clr] << '\n';
					}
					else if (p == dragon)//dragon
					{
						warrior[clr][number[clr]].morale = (double)life[clr] / (double)a[p];
						cout << "Its morale is " << fixed << setprecision(2) << warrior[clr][number[clr]].morale << '\n';
					}
					else if (p == iceman)//iceman
						warrior[clr][number[clr]].walk = true;
				}
			}
			timee += 5;
			if (timee > T)break;
			/*
				lion逃跑
				000:05 blue lion 1 ran away
			*/
			for (int i = 0;i <= N + 1;i++)
			{
				for (int clr = 0;clr < 2;clr++)
				{
					if ((i == 0 && clr == 1) || (i == N + 1 && clr == 0))continue;
					if (city[i][clr] != 0)
					{
						if (warrior[clr][city[i][clr]].attribute == 3 && warrior[clr][city[i][clr]].loyalty <= 0)//跑路了！
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
				if (warrior[1][city[1][1]].attribute == iceman)//iceman
				{
					warrior[1][city[1][1]].walk = !warrior[1][city[1][1]].walk;
					if (warrior[1][city[1][1]].walk)
					{
						if (warrior[1][city[1][1]].element > 9)
							warrior[1][city[1][1]].element -= 9;
						else
							warrior[1][city[1][1]].element = 1;
						warrior[1][city[1][1]].force += 20;
					}
				}
				coutime();cout << " blue " << warrior_name[warrior[1][city[1][1]].attribute] << " " << city[1][1] << " reached red headquarter with ";
				cout << warrior[1][city[1][1]].element << " elements and force " << warrior[1][city[1][1]].force << '\n';//蓝方抵达红方司令部
				occupy[red]++;
				if (occupy[red] == 2)
				{
					coutime();cout << " red headquarter was taken" << '\n';//红方司令部被占领
					end = 1;
				}
			}
			for (int cty = 1;cty <= N;cty++)
			{
				if (city[cty - 1][0] != 0)
				{
					if (warrior[0][city[cty - 1][0]].attribute == iceman)//iceman
					{
						warrior[0][city[cty - 1][0]].walk = !warrior[0][city[cty - 1][0]].walk;
						if (warrior[0][city[cty - 1][0]].walk)
						{
							if (warrior[0][city[cty - 1][0]].element > 9)
								warrior[0][city[cty - 1][0]].element -= 9;
							else
								warrior[0][city[cty - 1][0]].element = 1;
							warrior[0][city[cty - 1][0]].force += 20;
						}
					}
					coutime();cout << " red " << warrior_name[warrior[0][city[cty - 1][0]].attribute] << " " << city[cty - 1][0] << " marched to city " << cty << " with ";
					cout << warrior[0][city[cty - 1][0]].element << " elements and force " << warrior[0][city[cty - 1][0]].force << '\n';
				}
				if (city[cty + 1][1] != 0)
				{
					if (warrior[1][city[cty + 1][1]].attribute == iceman)//iceman
					{
						warrior[1][city[cty + 1][1]].walk = !warrior[1][city[cty + 1][1]].walk;
						if (warrior[1][city[cty + 1][1]].walk)
						{
							if (warrior[1][city[cty + 1][1]].element > 9)
								warrior[1][city[cty + 1][1]].element -= 9;
							else
								warrior[1][city[cty + 1][1]].element = 1;
							warrior[1][city[cty + 1][1]].force += 20;
						}
					}
					coutime();cout << " blue " << warrior_name[warrior[1][city[cty + 1][1]].attribute] << " " << city[cty + 1][1] << " marched to city " << cty << " with ";
					cout << warrior[1][city[cty + 1][1]].element << " elements and force " << warrior[1][city[cty + 1][1]].force << '\n';
				}
			}
			if (city[N][0] > 0)
			{
				if (warrior[0][city[N][0]].attribute == 2)//iceman
				{
					warrior[0][city[N][0]].walk = !warrior[0][city[N][0]].walk;
					if (warrior[0][city[N][0]].walk)
					{
						if (warrior[0][city[N][0]].element > 9)
							warrior[0][city[N][0]].element -= 9;
						else
							warrior[0][city[N][0]].element = 1;
						warrior[0][city[N][0]].force += 20;
					}
				}
				coutime();cout << " red " << warrior_name[warrior[0][city[N][0]].attribute] << " " << city[N][0] << " reached blue headquarter with ";
				cout << warrior[0][city[N][0]].element << " elements and force " << warrior[0][city[N][0]].force << '\n';//红方抵达蓝方司令部
				occupy[blue]++;
				if (occupy[blue] == 2)
				{
					coutime();cout << " blue headquarter was taken" << '\n';//蓝方司令部被占领
					end = 1;
				}
			}
			if (end)break;
			//修改city数组和武士位置
			for (int clr = 0;clr < 2;clr++)
			{
				for (int i = 1;i <= number[clr];i++)
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
			timee += 20;
			if (timee > T)break;
			//在每个小时的第20分：每个城市产出10个生命元。生命元留在城市，直到被武士取走。
			for (int cty = 1;cty <= N;cty++)city_history[cty][0] += 10;
			/*
				在每个小时的第30分：如果某个城市中只有一个武士，那么该武士取走该城市中的所有生命元，并立即将这些生命元传送到其所属的司令部。
				000:30 blue lion 1 earned 10 elements for his headquarter
			*/
			for (int cty = 1;cty <= N;cty++)
			{
				for (int clr = 0;clr < 2;clr++)
				{
					if (city[cty][clr] > 0 && city[cty][1 - clr] == 0)
					{
						warriors& wrr = warrior[clr][city[cty][clr]];
						coutime();cout << " " << color_name[clr] << " " << warrior_name[wrr.attribute] << " " << wrr.id << " earned " << city_history[cty][0] << " elements for his headquarter" << '\n';
						life[clr] += city_history[cty][0];city_history[cty][0] = 0;
					}
				}
			}
			timee += 5;
			if (timee > T)break;
			/*
				武士放矢
				输出样例： 000:35 blue dragon 1 shot
				表示在 0点35分，编号为1的蓝魔dragon武士射出一支箭。如果射出的箭杀死了敌人，则应如下输出：
				000:35 blue dragon 1 shot and killed red lion 4
				表示在 0点35分，编号为1的蓝魔dragon武士射出一支箭，杀死了编号为4的红魔lion。
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
			for (int cty = 1;cty <= N;cty++)
			{
				for (int clr = 0;clr < 2;clr++)
				{
					if (city[cty][clr] > 0 && warrior[clr][city[cty][clr]].weapon[arrow])
					{
						int move[2] = { 1,-1 };
						int next = cty + move[clr];
						if (next > 0 && next <= N && city[next][1 - clr] > 0)
						{
							warriors& wrr1 = warrior[clr][city[cty][clr]], & wrr2 = warrior[1 - clr][city[next][1 - clr]];
							coutime();cout << " " << color_name[clr] << " " << warrior_name[wrr1.attribute] << " " << wrr1.id << " shot";
							wrr1.arrow--;
							if (wrr1.arrow == 0)
								wrr1.weapon[arrow] = false;
							wrr2.element -= R;
							if (wrr2.element <= 0)
								cout << " and killed " << color_name[1 - clr] << " " << warrior_name[wrr2.attribute] << " " << wrr2.id;
							cout << '\n';
						}
					}
				}
			}
			timee += 3;
			if (timee > T)break;
			/*
				武士同归于尽
				输出样例： 000:38 blue dragon 1 used a bomb and killed red lion 7
				表示在 0点38分，编号为1的蓝魔dragon武士用炸弹和编号为7的红魔lion同归于尽。
			*/
			for (int i = 0;i < war_num;i++)
			{
				if (warrior[0][city[war_city[i]][0]].element > 0 && warrior[1][city[war_city[i]][1]].element > 0)
				{
					for (int clr = 0;clr < 2;clr++)
					{
						if (warrior[clr][city[war_city[i]][clr]].weapon[bomb])
						{
							warriors warrior_simu[2] = { warrior[0][city[war_city[i]][0]],warrior[1][city[war_city[i]][1]] };
							/*
								在插红旗的城市，以及编号为奇数的无旗城市，由红武士主动发起进攻。
								在插蓝旗的城市，以及编号为偶数的无旗城市，由蓝武士主动发起进攻。
							*/
							bool cast = false;
							int first = 1;
							switch (city_history[war_city[i]][3])
							{
							case 0:if (war_city[i] % 2)first = 0;break;
							case 1:first = 0;break;
							default:break;
							}
							if (fight(&warrior_simu[first], &warrior_simu[1 - first]))
							{
								if (clr != first)cast = true;
							}
							else if (warrior_simu[1 - first].attribute != ninja && fight(&warrior_simu[1 - first], &warrior_simu[first], true))
							{
								if (clr == first)cast = true;
							}
							if (cast)//同归于尽
							{
								//000:38 blue dragon 1 used a bomb and killed red lion 7
								coutime();cout << " " << color_name[clr] << " " << warrior_name[warrior[clr][city[war_city[i]][clr]].attribute] << " " << warrior[clr][city[war_city[i]][clr]].id;
								cout << " used a bomb and killed " << color_name[1 - clr] << " " << warrior_name[warrior[1 - clr][city[war_city[i]][1 - clr]].attribute] << " " << warrior[1 - clr][city[war_city[i]][1 - clr]].id << '\n';
								warrior[clr][city[war_city[i]][clr]].element = 0;warrior[1 - clr][city[war_city[i]][1 - clr]].element = 0;
								break;//只可能一方扔炸弹（因为预测的胜负是确定的）否则因为修改了实际数据，再模拟蓝方时可能出现蓝方也扔炸弹的笑话[捂脸]
							}
						}
					}
				}
			}
			timee += 2;
			if (timee > T)break;
			/*
				主动进攻：
				输出样例：000:40 red iceman 1 attacked blue lion 1 in city 1 with 20 elements and force 30
				表示在0点40分，1号城市中，红魔1号武士iceman 进攻蓝魔1号武士lion,在发起进攻前，红魔1号武士iceman生命值为20，攻击力为 30

				反击：
				输出样例：001:40 blue dragon 2 fought back against red lion 2 in city 1
				表示在1点40分，1号城市中，蓝魔2号武士dragon反击红魔2号武士lion

				战死：
				输出样例：001:40 red lion 2 was killed in city 1
				被箭射死的武士就不会有这一条输出。

				欢呼：
				输出样例：003:40 blue dragon 2 yelled in city 4

				获取生命元：
				输出样例：001:40 blue dragon 2 earned 10 elements for his headquarter
				输出不包括在30分不是通过战斗获取的elements

				旗帜升起：
				输出样例：004:40 blue flag raised in city 4

			*	wolf缴获武器

			*	司令部总是先完成全部奖励工作，然后才开始从各个打了胜仗的城市回收生命元。

			*/
			int add_element[2] = { 0 };//回收的生命元
			bool hortation[22] = { false };//射死没奖励，0到war_num-1
			for (int i = 0;i < war_num;i++)
			{
				int winner_camp = -1;
				warriors* wrr[2] = { &warrior[0][city[war_city[i]][0]] ,&warrior[1][city[war_city[i]][1]] };
				int element[2] = { wrr[0]->element,wrr[1]->element };
				int first = 1;
				switch (city_history[war_city[i]][3])
				{
				case 0:if (war_city[i] % 2)first = 0;break;
				case 1:first = 0;break;
				default:break;
				}
				for (int clr = 0;clr < 2;clr++)
				{
					if (wrr[clr]->attribute == dragon)
						wrr[clr]->morale -= 0.2;//如果赢了后面会加回去
				}
				if (element[0] > 0 && element[1] > 0)//都活着
				{
					coutime();cout << " " << color_name[first] << " " << warrior_name[wrr[first]->attribute] << " " << wrr[first]->id << " attacked " << color_name[1 - first] << " " << warrior_name[wrr[1 - first]->attribute];
					cout << " " << wrr[1 - first]->id << " in city " << war_city[i] << " with " << wrr[first]->element << " elements and force " << wrr[first]->force << '\n';
					if (fight(wrr[first], wrr[1 - first]))
						winner_camp = first;
					else if (wrr[1 - first]->attribute != ninja)
					{
						coutime();cout << " " << color_name[1 - first] << " " << warrior_name[wrr[1 - first]->attribute] << " " << wrr[1 - first]->id << " fought back against ";
						cout << color_name[first] << " " << warrior_name[wrr[first]->attribute] << " " << wrr[first]->id << " in city " << war_city[i] << '\n';
						if (fight(wrr[1 - first], wrr[first], true))
							winner_camp = 1 - first;
					}
					if (winner_camp != -1)
					{
						coutime();cout << " " << color_name[1 - winner_camp] << " " << warrior_name[wrr[1 - winner_camp]->attribute] << " " << wrr[1 - winner_camp]->id << " was killed in city " << war_city[i] << '\n';
					}
				}
				else if (element[red] > 0)
					winner_camp = red;
				else if (element[blue] > 0)
					winner_camp = blue;
				/*
					dragon 在一次在它主动进攻的战斗结束后，如果还没有战死，而且士气值大于0.8，就会欢呼。
					dragon每取得一次战斗的胜利(敌人被杀死)，士气就会增加0.2，每经历一次未能获胜的战斗，士气值就会减少0.2。
					士气增减发生在欢呼之前。
				*/
				switch (winner_camp)
				{
				case -1:
					if (wrr[first]->attribute == dragon && wrr[first]->element > 0 && wrr[first]->morale > 0.8)
					{
						coutime();cout << " " << color_name[first] << " dragon " << wrr[first]->id << " yelled in city " << war_city[i] << '\n';
					}
					for (int clr = 0;clr < 2;clr++)
					{
						if (wrr[clr]->attribute == lion)//愣是忘了这个
							wrr[clr]->loyalty -= K;
					}
					//武士使用bomb和敌人同归于尽的情况下，不算是一场战斗，双方都不能拿走城市的生命元，也不影响城市的旗帜。//靠官方测试数据才发现的。。。
					if (wrr[0]->element > 0 || wrr[1]->element > 0)
					{
						city_history[war_city[i]][1] = city_history[war_city[i]][2];
						city_history[war_city[i]][2] = 3;//平局
					}
					break;
				default:
					hortation[i] = true;
					if (wrr[1 - winner_camp]->attribute == lion && element[1 - winner_camp] > 0)//element[1 - winner_camp]没想到啊
						wrr[winner_camp]->element += element[1 - winner_camp];
					if (wrr[winner_camp]->attribute == wolf)
					{
						for (int weapon = 0;weapon < 3;weapon++)
						{
							if (!wrr[winner_camp]->weapon[weapon] && wrr[1 - winner_camp]->weapon[weapon])
							{
								wrr[winner_camp]->weapon[weapon] = true;
								switch (weapon)
								{
								case sword:wrr[winner_camp]->sword = wrr[1 - winner_camp]->sword;break;
								case arrow:wrr[winner_camp]->arrow = wrr[1 - winner_camp]->arrow;break;
								default:break;
								}
							}
						}
					}
					else if (first == winner_camp && wrr[winner_camp]->attribute == dragon)
					{
						wrr[winner_camp]->morale += 0.4;
						if (wrr[winner_camp]->morale > 0.8)
						{
							coutime();cout << " " << color_name[winner_camp] << " dragon " << wrr[winner_camp]->id << " yelled in city " << war_city[i] << '\n';
						}
					}
					coutime();cout << " " << color_name[winner_camp] << " " << warrior_name[wrr[winner_camp]->attribute] << " " << wrr[winner_camp]->id << " earned " << city_history[war_city[i]][0] << " elements for his headquarter" << '\n';
					add_element[winner_camp] += city_history[war_city[i]][0];
					city_history[war_city[i]][0] = 0;
					city_history[war_city[i]][1] = city_history[war_city[i]][2];
					if (city_history[war_city[i]][3] != winner_camp + 1 && city_history[war_city[i]][2] == winner_camp + 1)
					{
						city_history[war_city[i]][3] = winner_camp + 1;
						coutime();cout << " " << color_name[winner_camp] << " flag raised in city " << war_city[i] << '\n';
					}
					city_history[war_city[i]][2] = winner_camp + 1;
					break;
				}
			}
			for (int i = 0;i < war_num;i++)
			{
				if (life[blue] >= 8)
				{
					if (hortation[i])
					{
						warriors& wrr_red = warrior[0][city[war_city[i]][0]], & wrr_blue = warrior[1][city[war_city[i]][1]];
						if (wrr_blue.element > 0 && wrr_red.element <= 0)
						{
							wrr_blue.element += 8;life[blue] -= 8;
						}
					}
				}
				else break;
			}
			for (int i = war_num - 1;i >= 0;i--)
			{
				if (life[red] >= 8)
				{
					if (hortation[i])
					{
						warriors& wrr_red = warrior[0][city[war_city[i]][0]], & wrr_blue = warrior[1][city[war_city[i]][1]];
						if (wrr_blue.element <= 0 && wrr_red.element > 0)
						{
							wrr_red.element += 8;life[red] -= 8;
						}
					}
				}
				else break;
			}
			life[red] += add_element[red];life[blue] += add_element[blue];
			//修改city数组
			for (int i = 0;i <= N;i++)
			{
				for (int clr = 0;clr < 2;clr++)
				{
					if (city[i][clr] > 0 && warrior[clr][city[i][clr]].element <= 0)
						city[i][clr] = 0;
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
				000:55 blue wolf 2 has arrow(2),bomb,sword(23)
				000:55 blue wolf 4 has no weapon
				000:55 blue wolf 5 has sword(20)

				在0点55分，蓝魔2号武士wolf有一支arrow（这支arrow还可以用2次），一个bomb，还有一支攻击力为23的sword。
				蓝魔4号武士wolf没武器。
				蓝魔5号武士wolf有一支攻击力为20的sword。

				交代武器情况时，次序依次是：arrow,bomb,sword。
				如果没有某种武器，某种武器就不用提。
				报告时，先按从西向东的顺序所有的红武士报告，然后再从西向东所有的蓝武士报告。
			*/
			for (int clr = 0;clr < 2;clr++)
			{
				for (int i = 0;i <= N + 1;i++)
				{
					if (city[i][clr] != 0)
					{
						warriors& wrr = warrior[clr][city[i][clr]];
						coutime();cout << " " << color_name[clr] << " " << warrior_name[wrr.attribute] << " " << wrr.id << " has ";
						bool has = 0;
						for (int w = 2;w >= 0;w--)
						{
							if (wrr.weapon[w])
							{
								if (has)cout << ",";
								else has = true;
								cout << weapon_name[w];
								switch (w)
								{
								case arrow:cout << "(" << wrr.arrow << ")";break;
								case sword:cout << "(" << wrr.sword << ")";break;
								default:break;
								}
							}
						}
						if (!has)
							cout << "no weapon";
						cout << '\n';
					}
				}
			}
			timee += 5;
			if (timee > T)break;
		}
	}
	return 0;
}
