#include<string>
#include<cstring>
#include<iomanip>
#include<iostream>
using namespace std;

enum WARRIOR_{
	dragon, ninja, iceman, lion, wolf
};
enum WEAPON_{
	sword_, bomb_, arrow_
};
enum COLOR_{
	red, blue
};

char warrior_name[5][8] = { "dragon","ninja","iceman","lion","wolf" };//武士的名字
char weapon_name[3][8] = { "sword","bomb","arrow" };
char color_name[2][8] = { "red","blue" };

int N,R,K;

class TIME{
    static int time;
    static int maxtime;
public:
    static bool add(int n){
        time+=n;
        if(time>maxtime)return true;
        return false;
    }
    static void set(int max){maxtime=max;time=0;}
	static std::ostream&Time(std::ostream&out){
	    int t = time / 60;int dgt = 0;
	    while (t) { dgt++;t = t / 10; }
	    while (3 - dgt > 0) { out << 0;dgt++; }
	    if (time / 60)out << time / 60;
	    out << ":";
	    t = time % 60;
	    if (t / 10 == 0)out << 0;
	    out << t;
		return out;
	}
};
int TIME::time=0;
int TIME::maxtime=23333;

class WARRIOR;
class HEADQUARTER;
class RED;
class BLUE;

class CITY{
	static WARRIOR* city[22][2];
    static int history[22][4];
	static int war_city[22];
	static int war_num;
	public:
	static void set(){
		for(int i=0;i<22;++i)
		for(int j=0;j<2;++j)
		city[i][j]=NULL;
		for(int i=0;i<22;++i)
		for(int j=0;j<4;++j)
		history[i][j]=0;
	}
	static void add(WARRIOR*w);
	static bool move(RED*red_head,BLUE*blu_head);
	static void run();
	static void bonus(){
		for (int cty = 1;cty <= N;cty++)history[cty][0] += 10;
	}
	static void baigei(HEADQUARTER*head[]);
	static void beizhan(){
		war_num=0;
		for (int i = 1;i <= N;i++)
			if (city[i][0]!=NULL && city[i][1]!=NULL)
				war_city[war_num] = i,war_num++;
	}
	static void jijian();
	static void tongguiyujin();
	static void war(HEADQUARTER*head[]);
	static void huibao();
};
WARRIOR* CITY::city[22][2]={NULL};
int CITY::history[22][4]={0};
int CITY::war_city[22]={0};
int CITY::war_num=0;


class HEADQUARTER{
	friend class CITY;
    protected:
    int element;
    int camp;
    static int a[5];
    static int b[5];
    int number;
	int occupied;
    WARRIOR*warriors[100];
    HEADQUARTER(int x,int y):element(x),camp(y),number(0),occupied(0){
		for(int i=0;i<100;++i)warriors[i]=NULL;
	}
	public:
    static void set(){
        for (int i = 0;i < 5;i++)cin >> a[i];//生命值
		for (int i = 0;i < 5;i++)cin >> b[i];//攻击力
    }
	virtual~HEADQUARTER(){}
	public:
	virtual void baogao(){}
};
int HEADQUARTER::a[5]={0};
int HEADQUARTER::b[5]={0};

class WARRIOR{
friend class WOLF;
friend class NINJA;
friend class LION;
friend class DRAGON;
protected:
	friend class CITY;
	int attribute;
    int camp;
    int id;
    int element;
    int force;
    int place;
    bool weapon[3]={0};
    int arrow;
    int sword;
    WARRIOR(int camp_,int id_,int element_,int force_,int att):
		camp(camp_),id(id_),element(element_),force(force_),attribute(att),arrow(0),sword(0){
		for(int i=0;i<3;++i)weapon[i]=0;
        if(camp==red)place=0;
        else place=N+1;
		CITY::add(this);
    }
	void unsword_or_arrow(){//sword判0处理&&arrow处理
		if(weapon[sword]){
			int fierce = force / 5;
			if (fierce == 0)
				weapon[sword] = false;
			else
				sword = fierce;
		}
		if(weapon[arrow_])arrow=3;
	}
	virtual bool run(){return false;}
	virtual void walk_(){}
	virtual void do_(){}
	virtual void morale_(double p){}
	virtual void huibao(){
		cout<<TIME::Time << " " << *this << " has ";
		bool has = 0;
		for (int w = 2;w >= 0;w--){
			if (weapon[w]){
				if (has)cout << ",";
				else has = true;
				cout << weapon_name[w];
				switch (w){
				case arrow_:cout << "(" << arrow << ")";break;
				case sword_:cout << "(" << sword << ")";break;
				default:break;
				}
			}
		}
		if (!has)
			cout << "no weapon";
		cout << '\n';
	}
	void march(int city,bool end=false){
		place=city;
		walk_();
		if(end){
			cout <<TIME::Time<< ' '<<color_name[camp]<<' ' << warrior_name[attribute] << " " << id << " reached "<<color_name[1-camp]<<" headquarter with ";
			cout << element << " elements and force " << force << '\n';//抵达司令部
		}
		else{
			cout <<TIME::Time<< ' '<<color_name[camp]<<' ' << warrior_name[attribute] << " " << id << " marched to city " << city << " with ";
			cout << element << " elements and force " << force << '\n';
		}
	}
	public:
	virtual void steal(WARRIOR*w){}
    virtual~WARRIOR(){}
	friend std::ostream&operator<<(std::ostream&out,const WARRIOR&w);
	static bool fight(WARRIOR* gong, WARRIOR* shou, bool re=false);//某战士攻击对手一次，杀死敌人返回1
};

std::ostream&operator<<(std::ostream&out,const WARRIOR&w){
	out<<color_name[w.camp]<<' '<<warrior_name[w.attribute]<<' '<<w.id;
	return out;
}

class DRAGON:public WARRIOR{
    static char name[7];
    double morale;
    public:
    DRAGON(int camp,int num,int ele,int fce,double mor):WARRIOR(camp,num,ele,fce,dragon),morale(mor){
		weapon[num%3]=true;
		unsword_or_arrow();
		cout << "Its morale is " << std::fixed << std::setprecision(2) << morale << '\n';
	}
	void morale_(double p){morale+=p;}
	void do_(){
		if(morale > 0.8)
			cout <<TIME::Time<< " " << *this << " yelled in city " << place << '\n';
	}
};
char DRAGON::name[7]="dragon";


class NINJA:public WARRIOR{
    static char name[6];
public:
	NINJA(int camp,int num,int ele,int fce):WARRIOR(camp,num,ele,fce,ninja){
		weapon[num%3]=true;
		weapon[(num+1)%3]=true;
		unsword_or_arrow();
	}

};
char NINJA::name[6]="ninja";


class ICEMAN:public WARRIOR{
    static char name[7];
    bool walk;
public:
	ICEMAN(int camp,int num,int ele,int fce):WARRIOR(camp,num,ele,fce,iceman),walk(true){
		weapon[num%3]=true;
		unsword_or_arrow();
	}
	void walk_(){
		walk=!walk;
		if(walk){
			if(element>9)element-=9;
			else element=1;
			force+=20;
		}
	}
};
char ICEMAN::name[7]="iceman";


class LION:public WARRIOR{
    static char name[5];
    int loyalty;
public:
	LION(int camp,int num,int ele,int fce,int lol):WARRIOR(camp,num,ele,fce,lion),loyalty(lol){
		cout << "Its loyalty is " << loyalty << '\n';
	}
	bool run(){
		if(loyalty<=0){
			element=0;
			cout <<TIME::Time<< " " << color_name[camp] << " lion " << id << " ran away" << '\n';
			return true;
		}
		return false;
	}
	void do_(){
		loyalty -= K;
	}
};
char LION::name[5]="lion";


class WOLF:public WARRIOR{
    static char name[5];
	public:
	WOLF(int camp,int num,int ele,int fce):WARRIOR(camp,num,ele,fce,wolf){}
	void steal(WARRIOR*w){
		for (int wp = 0;wp < 3;wp++){
			if (!weapon[wp] && w->weapon[wp]){
				weapon[wp] = true;
				switch (wp){
				case sword_:sword = w->sword;break;
				case arrow_:arrow = w->arrow;break;
				default:break;
				}
			}
		}
	}
};
char WOLF::name[5]="wolf";


class RED:public HEADQUARTER{
	friend class CITY;
	static char name[4];
    static int c[5];
    static int cid;
    public:
    RED(int life):HEADQUARTER(life,red){cid=4;}
    void make(){
        cid = (cid + 1) % 5;//制造顺序指示
		int p = c[cid];//武士属性attribute
		if (a[p] > element)//不能制造武士
			cid = (cid + 4) % 5;
		else{
			number++;//某方武士总数
			element -= a[p];
			cout <<TIME::Time<< " " << name << " " << warrior_name[p] << " " << number << " born" << '\n';
            switch (p){
                case dragon:warriors[number]=new DRAGON(camp,number,a[dragon],b[dragon],(double)element / (double)a[dragon]);break;
                case ninja:warriors[number]=new NINJA(camp,number,a[ninja],b[ninja]);break;
                case iceman:warriors[number]=new ICEMAN(camp,number,a[iceman],b[iceman]);break;
                case lion:warriors[number]=new LION(camp,number,a[lion],b[lion],element);break;
                case wolf:warriors[number]=new WOLF(camp,number,a[wolf],b[wolf]);break;
                default:break;
            }
		}	
    }
	void baogao(){
		cout <<TIME::Time <<" " << element<< " elements in red headquarter" << '\n';
	}
	~RED(){
		for(int i=number;i;--i)
		delete (warriors[i]);
	}
};
int RED::c[5]={2,3,4,1,0};
char RED::name[4]="red";
int RED::cid=4;


class BLUE:public HEADQUARTER{
	friend class CITY;
	static char name[5];
    static int c[5];
    static int cid;
    public:
    WARRIOR*warriors[100];
    BLUE(int life):HEADQUARTER(life,blue){cid=4;}
	void make(){
        cid = (cid + 1) % 5;//制造顺序指示
		int p = c[cid];//武士属性attribute
		if (a[p] > element)//不能制造武士
			cid = (cid + 4) % 5;
		else{
			number++;//某方武士总数
			element -= a[p];
			cout << TIME::Time<<" " << name << " " << warrior_name[p] << " " << number << " born" << '\n';
            switch (p){
                case dragon:warriors[number]=new DRAGON(camp,number,a[dragon],b[dragon],(double)element / (double)a[dragon]);break;
                case ninja:warriors[number]=new NINJA(camp,number,a[ninja],b[ninja]);break;
                case iceman:warriors[number]=new ICEMAN(camp,number,a[iceman],b[iceman]);break;
                case lion:warriors[number]=new LION(camp,number,a[lion],b[lion],element);break;
                case wolf:warriors[number]=new WOLF(camp,number,a[wolf],b[wolf]);break;
                default:break;
            }
		}	
    }
	void baogao(){
		cout <<TIME::Time <<" " << element<< " elements in blue headquarter" << '\n';
	}
	~BLUE(){
		for(int i=number;i;--i)
		delete (warriors[i]);
	}
};
int BLUE::c[5]={3,0,1,2,4};
char BLUE::name[5]="blue";
int BLUE::cid=4;


void CITY::add(WARRIOR*w){
	city[w->place][w->camp]=w;
}

bool CITY::move(RED*red_head,BLUE*blu_head){
	bool ans=false;
	if (city[1][1]!=NULL){
		city[1][1]->march(0,true);
		red_head->occupied++;
		if (red_head->occupied == 2){
			cout <<TIME::Time<< " red headquarter was taken" << '\n';//红方司令部被占领
			ans = true;
		}
	}
	for (int cty = 1;cty <= N;cty++){
		if (city[cty - 1][0] != NULL)city[cty - 1][0]->march(cty);
		if (city[cty + 1][1] != NULL)city[cty + 1][1]->march(cty);
	}
	if (city[N][0]!=NULL){
		city[N][0]->march(N+1,true);
		blu_head->occupied++;
		if (blu_head->occupied == 2){
			cout <<TIME::Time<< " blue headquarter was taken" << '\n';
			ans = true;
		}
	}
	//修改city数组
	if(city[0][1]==NULL)city[0][1]=city[1][1];
	for(int i=1;i<=N;++i)
		city[i][1]=city[i+1][1];
	city[N+1][1]=NULL;
	if(city[N+1][0]==NULL)city[N+1][0]=city[N][0];
	for(int i=N;i;--i)
		city[i][0]=city[i-1][0];
	city[0][0]=NULL;
	return ans;		
}

void CITY::run(){
	for (int i = 0;i <= N + 1;i++){
		for (int clr = 0;clr < 2;clr++){
			if ((i == 0 && clr == 1) || (i == N + 1 && clr == 0))continue;
			if (city[i][clr] != NULL)if(city[i][clr]->run())city[i][clr] = NULL;
		}
	}
}

void CITY::baigei(HEADQUARTER*head[]){
	for (int cty = 1;cty <= N;cty++){
		for (int clr = 0;clr < 2;clr++){
			if (city[cty][clr]!=NULL&& city[cty][1 - clr]==NULL){
				cout <<TIME::Time<< " " << *(city[cty][clr]) << " earned " << history[cty][0] << " elements for his headquarter" << '\n';
				head[clr]->element += history[cty][0];history[cty][0] = 0;break;
			}
		}
	}
}

void CITY::jijian(){
	for (int cty = 1;cty <= N;cty++){
		for (int clr = 0;clr < 2;clr++){
			if (city[cty][clr]!=NULL && city[cty][clr]->weapon[arrow_]){
				int move[2] = { 1,-1 };
				int next = cty + move[clr];
				if (next > 0 && next <= N && city[next][1 - clr]!=NULL){
					WARRIOR* wrr1 = city[cty][clr], * wrr2 = city[next][1 - clr];
					cout <<TIME::Time<< " " << *wrr1 << " shot";
					wrr1->arrow--;
					if (wrr1->arrow == 0)
						wrr1->weapon[arrow_] = false;
					wrr2->element -= R;
					if (wrr2->element <= 0){
						cout << " and killed " << *wrr2;
					}
					cout << '\n';
				}
			}
		}
	}
}

void CITY::tongguiyujin(){
	for (int i = 0;i < war_num;i++){
		if (city[war_city[i]][0]->element<=0||city[war_city[i]][1]->element<=0)
			continue;
		for (int clr = 0;clr < 2;clr++){
			if (city[war_city[i]][clr]->weapon[bomb_]){
				WARRIOR warrior_simu[2] = { *city[war_city[i]][0],*city[war_city[i]][1] };
				/*
					在插红旗的城市，以及编号为奇数的无旗城市，由红武士主动发起进攻。
					在插蓝旗的城市，以及编号为偶数的无旗城市，由蓝武士主动发起进攻。
				*/
				bool cast = false;
				int first = 1;
				switch (history[war_city[i]][3]){
				case 0:if (war_city[i] % 2)first = 0;break;
				case 1:first = 0;break;
				default:break;
				}
				if (WARRIOR::fight(&warrior_simu[first], &warrior_simu[1 - first])){
					if (clr != first)cast = true;
				}
				else if (warrior_simu[1 - first].attribute != ninja && WARRIOR::fight(&warrior_simu[1 - first], &warrior_simu[first], true)){
					if (clr == first)cast = true;
				}
				if (cast){//同归于尽
					//000:38 blue dragon 1 used a bomb and killed red lion 7
					cout<<TIME::Time << " " << warrior_simu[clr];
					cout << " used a bomb and killed " << warrior_simu[1-clr]<< '\n';
					city[war_city[i]][clr]->element = 0;city[war_city[i]][1 - clr]->element = 0;
					break;//只可能一方扔炸弹（因为预测的胜负是确定的）否则因为修改了实际数据，再模拟蓝方时可能出现蓝方也扔炸弹的笑话[捂脸]
				}
			}
		}
	}
}

bool WARRIOR::fight(WARRIOR* gong, WARRIOR* shou, bool re){//某战士攻击对手一次，杀死敌人返回1
	int f = re ? gong->force / 2 : gong->force;
	if (gong->weapon[sword_])
	{
		f += gong->sword;
		gong->sword = gong->sword * 4 / 5;
		if (gong->sword == 0)
			gong->weapon[sword_] = false;
	}
	shou->element -= f;
	if (shou->element <= 0)
		return 1;
	return 0;
}

void CITY::war(HEADQUARTER*head[]){
	int add_element[2] = { 0 };//回收的生命元
	bool hortation[22] = { false };//射死没奖励，0到war_num-1
	for (int i = 0;i < war_num;i++){
		int winner_camp = -1;
		WARRIOR* wrr[2] = { city[war_city[i]][0] ,city[war_city[i]][1] };
		int element[2] = { wrr[0]->element,wrr[1]->element };
		int first = 1;
		switch (history[war_city[i]][3]){
		case 0:if (war_city[i] % 2)first = 0;break;
		case 1:first = 0;break;
		default:break;
		}
		for (int clr = 0;clr < 2;clr++){
			if (wrr[clr]->attribute == dragon)
				wrr[clr]->morale_(0.2);//如果赢了后面会加回去
		}
		if (element[0] > 0 && element[1] > 0){//都活着
			cout <<TIME::Time<< " " << *wrr[first] << " attacked " << *wrr[1-first]<< " in city " << war_city[i] << " with " << wrr[first]->element << " elements and force " << wrr[first]->force << '\n';
			if (WARRIOR::fight(wrr[first], wrr[1 - first]))
				winner_camp = first;
			else if (wrr[1 - first]->attribute != ninja){
				cout <<TIME::Time<< " " << *wrr[1 - first] << " fought back against "<<*wrr[first] << " in city " << war_city[i] << '\n';
				if (WARRIOR::fight(wrr[1 - first], wrr[first], true))
					winner_camp = 1 - first;
			}
			if (winner_camp != -1){
				cout <<TIME::Time<< " " << *wrr[1-winner_camp] << " was killed in city " << war_city[i] << '\n';
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
		switch (winner_camp){
		case -1:
			if (wrr[first]->attribute == dragon && wrr[first]->element > 0 )
				wrr[first]->do_();
			for (int clr = 0;clr < 2;clr++)
				if (wrr[clr]->attribute == lion)//愣是忘了这个
					wrr[clr]->do_();
			//武士使用bomb和敌人同归于尽的情况下，不算是一场战斗，双方都不能拿走城市的生命元，也不影响城市的旗帜。//靠官方测试数据才发现的。。。
			if (wrr[0]->element > 0 || wrr[1]->element > 0){
				history[war_city[i]][1] = history[war_city[i]][2];
				history[war_city[i]][2] = 3;//平局
			}
			break;
		default:
			hortation[i] = true;
			if (wrr[1 - winner_camp]->attribute == lion && element[1 - winner_camp] > 0)//element[1 - winner_camp]没想到啊
				wrr[winner_camp]->element += element[1 - winner_camp];
			if (wrr[winner_camp]->attribute == wolf)
				wrr[winner_camp]->steal(wrr[1-winner_camp]);
			else if (first == winner_camp && wrr[winner_camp]->attribute == dragon){
				wrr[winner_camp]->morale_(0.4);
				wrr[winner_camp]->do_();
			}
			cout <<TIME::Time<< " " << *wrr[winner_camp] << " earned " << history[war_city[i]][0] << " elements for his headquarter" << '\n';
			add_element[winner_camp] += history[war_city[i]][0];
			history[war_city[i]][0] = 0;
			history[war_city[i]][1] = history[war_city[i]][2];
			if (history[war_city[i]][3] != winner_camp + 1 && history[war_city[i]][2] == winner_camp + 1){
				history[war_city[i]][3] = winner_camp + 1;
				cout <<TIME::Time<< " " << color_name[winner_camp] << " flag raised in city " << war_city[i] << '\n';
			}
			history[war_city[i]][2] = winner_camp + 1;
			break;
		}
	}
	for (int i = 0;i < war_num;i++){
		if (head[blue]->element >= 8){
			if (hortation[i]){
				if (city[war_city[i]][1]->element > 0 && city[war_city[i]][0]->element <= 0){
					city[war_city[i]][1]->element += 8;head[blue]->element -= 8;
				}
			}
		}
		else break;
	}
	for (int i = war_num - 1;i >= 0;i--){
		if (head[red]->element >= 8){
			if (hortation[i]){
				WARRIOR* wrr_red = city[war_city[i]][0], *wrr_blue = city[war_city[i]][1];
				if (wrr_blue->element <= 0 && wrr_red->element > 0){
					wrr_red->element += 8;head[red]->element -= 8;
				}
			}
		}
		else break;
	}
	head[red]->element += add_element[red];head[blue]->element += add_element[blue];
	//修改city数组
	for (int i = 0;i <= N;i++){
		for (int clr = 0;clr < 2;clr++){
			if (city[i][clr]!=NULL&& city[i][clr]->element <= 0)
				city[i][clr] = NULL;
		}
	}
			
}

void CITY::huibao(){
		for (int clr = 0;clr < 2;clr++){
			for (int i = 0;i <= N + 1;i++){
				if (city[i][clr] != NULL)
				city[i][clr]->huibao();
			}
		}
	}

int main(){
	cout.setf(std::ios::unitbuf); 
	int NM = 0;cin >> NM;
	for (int nm = 1;nm <= NM;nm++){
        int life,T;
		cin >> life >> N >> R >> K >> T;
		TIME::set(T);
		CITY::set();
		HEADQUARTER::set();//cin a[] b[]
		RED Red_HeadQuarter(life);
        BLUE Blue_HeadQuarter(life);
		HEADQUARTER* temp[2]={&Red_HeadQuarter,&Blue_HeadQuarter};
		cout << "Case " << nm << ":" << '\n';
		while (true){
			/*
				武士降生
				000:00 blue lion 1 born
				Its loyalty is 24
				000:00 blue dragon 1 born
				Its morale is 23.34
			*/
			Red_HeadQuarter.make();
			Blue_HeadQuarter.make();
			if(TIME::add(5))break;
			/*
				lion逃跑
				000:05 blue lion 1 ran away
			*/
			CITY::run();
			if(TIME::add(5))break;
			/*
				武士行进
				000:10 red iceman 1 marched to city 1 with 20 elements and force 30
				武士抵达敌军司令部
				001:10 red iceman 1 reached blue headquarter with 20 elements and force 30
				司令部被占领
				003:10 blue headquarter was taken
			*/
			if(CITY::move(&Red_HeadQuarter,&Blue_HeadQuarter))break;
			if(TIME::add(20))break;
			/*
				在每个小时的第20分：每个城市产出10个生命元。生命元留在城市，直到被武士取走。
				在每个小时的第30分：如果某个城市中只有一个武士，那么该武士取走该城市中的所有生命元，并立即将这些生命元传送到其所属的司令部。
				000:30 blue lion 1 earned 10 elements for his headquarter
			*/
			CITY::bonus();
			CITY::baigei(temp);
			if(TIME::add(5))break;
			/*
				武士放矢
				输出样例： 000:35 blue dragon 1 shot
				表示在 0点35分，编号为1的蓝魔dragon武士射出一支箭。如果射出的箭杀死了敌人，则应如下输出：
				000:35 blue dragon 1 shot and killed red lion 4
				表示在 0点35分，编号为1的蓝魔dragon武士射出一支箭，杀死了编号为4的红魔lion。
			*/
			CITY::beizhan();
			CITY::jijian();
			if(TIME::add(3))break;
			/*
				武士同归于尽
				输出样例： 000:38 blue dragon 1 used a bomb and killed red lion 7
				表示在 0点38分，编号为1的蓝魔dragon武士用炸弹和编号为7的红魔lion同归于尽。
			*/
			CITY::tongguiyujin();
			if(TIME::add(2))break;
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
				wolf缴获武器
				司令部总是先完成全部奖励工作，然后才开始从各个打了胜仗的城市回收生命元。
			*/
			CITY::war(temp);
			if(TIME::add(10))break;
			/*
				司令部报告
				000:50 100 elements in red headquarter
				000:50 120 elements in blue headquarter
			*/
			Red_HeadQuarter.baogao();
			Blue_HeadQuarter.baogao();
			if(TIME::add(5))break;
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
			CITY::huibao();
			if(TIME::add(5))break;

		}
	}
	return 0;
}
