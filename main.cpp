#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

#define MAX_PLANET 15
#define MAX_LEN 20
#define KEY1 100000000
#define KEY2 1000000
#define KEY3 100
/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
class Planet{
public:
    int boss;
    int gen;
    int cur;
	int base_gen=-1;
	int freeze=-1;
	int marker;
    void init(int b, int c, int g){
        boss=b;
        gen =g;
        cur =c;
		base_gen=max(gen,base_gen);
		if(gen<base_gen)
			if(freeze<0)freeze=5;
			else freeze--;
		else freeze=-1;
    }
};
 
class Troop{
public:
    int boss;
    int from;
    int to;
    int cur;
    int rem;
    Troop(int b, int f, int t, int c, int r){
        boss=b;
        from=f;
        to  =t;
        cur =c;
        rem =r;
    }
};

class Command{
public:
	int oper; //0-get;1-upgr;2-bomb;3-defence;4-antibomb;5-move to front;10-waiting enemy moves
	int from;
	int target;
	int counter;
	int guard;
	Command(int o, int f, int t, int g, int c){
		oper   =o;
		from   =f;
		target =t;
		counter=c;
		guard  =g;
	}
};

class Plan{
public:
	set<int> enemy_hsh;
	int rating;
};

class Solution{
private:
	int state; //0-init, 1-debut, 2-mid, 3-end
	int plan;
	int pl_id, m_cnt, e_cnt, n_cnt;
	int factoryCount;
	int ds[MAX_PLANET][MAX_PLANET];
	Planet all_p[MAX_PLANET];
	int reserved[MAX_PLANET];
	Plan plans[MAX_PLANET];
	vector<int> my_p,en_p,ne_p;
	
	vector<Troop> all_t;
	vector<int> my_t,en_t,ne_t;
	int to_planet_troop[MAX_PLANET][MAX_LEN];
	int future[MAX_PLANET][MAX_LEN];
	
	vector<Troop> bombs;
	
	vector<Command> cm,ce;
	int my_bomb, en_bomb;
	int turn;
public:
	Solution(int fC){
		turn=1;
		state=0;
		plan =0;
		my_bomb=2;
		en_bomb=2;
		factoryCount = fC;
		cm.clear();
		for(int i=0;i<factoryCount;i++)
			for(int j=0;j<factoryCount;j++)
			{
				ds[i][j]=0;
				ds[j][i]=0;
			}
	}
	void add_dist(int f, int t, int d){
		ds[f][t]=d;
		ds[t][f]=d;
	} 
	void add_planet(int b, int c, int g){
		all_p[pl_id].init(b,c,g);
		if(b==1){
			my_p.push_back(pl_id);
			all_p[pl_id].marker=0;
		}
		else if(b==-1)en_p.push_back(pl_id);
		else ne_p.push_back(pl_id);
		pl_id++;
	}
	void add_troop(int b, int f, int t, int c, int r){
		Troop tt(b,f,t,c,r);
		int tt_id=all_t.size();
		all_t.push_back(tt);
		to_planet_troop[t][r]+=c*b;
		if(b==1)my_t.push_back(tt_id);
		else en_t.push_back(tt_id);
	}
	void add_bomb(int b, int f, int t, int r){
		Troop bmb(b,f,t,0,r);
		cerr<<"BOMBA "<<b<<" "<<f<<" "<<t<<" "<<r<<endl;
		bombs.push_back(bmb);
	}
	void init(){
		pl_id=0;
		m_cnt=0;
		e_cnt=0;
		n_cnt=0;
		all_t.clear();
		my_t.clear();
		en_t.clear();
		my_p.clear();
		en_p.clear();
		bombs.clear();
		for(int i=0;i<factoryCount;i++)
			for(int j=0;j<MAX_LEN;j++){
				to_planet_troop[i][j]=0;
				future[i][j]=0;
			}
	}
	//------------------------------------------logic------------------------------------------
	void create_markers(){
		for(auto i : my_p){
			for(int j=0;j<factoryCount;j++){
				int d1,d2;
				d1=ds[i][j];
				d2=9999;
				for(auto k : en_p)
					d2=min(d2,ds[k][j]);
				if(d1<d2-3)all_p[j].marker=0;										// my zone
				else if(d2<d1-3)all_p[j].marker=10; 								//enemy
				else if(d1<d2)
					if(all_p[j].base_gen>1)
						if(all_p[j].cur>6)all_p[j].marker=2;						//my problem
						else all_p[j].marker=1;										//my optimal
					else all_p[j].marker=3;											//my nw
				else if(d2>d1)
					if(all_p[j].base_gen>1)
						if(all_p[j].cur>6)all_p[j].marker=12;						//en problem
						else all_p[j].marker=11;									//en optimal
					else all_p[j].marker=13;										//en nw
				else all_p[j].marker=100;											//mediane
			}
		}
	}
	void get_plan(){
		create_markers();
		
		vector<pair<int,int>> v_val,  v_need;
		int t_id=0,t_need,t_val;
		int my_planet=my_p[0];
		int deep=ds[my_planet][en_p[0]];
		if(deep<7){
			Command c(10,0,0,0,1);
			cm.push_back(c);
			return;
			// waiting enemy move
		}
		deep=min(deep,12);
		for(int i=0;i<factoryCount;i++){
			if((all_p[i].marker==0)&&(all_p[i].boss==0)){
				t_need=all_p[i].cur+1;
				t_val=1+(deep-ds[my_planet][i])*all_p[i].gen-t_need;
				v_need.push_back(make_pair(i,t_need));
				v_val.push_back(make_pair(t_val,t_id++));
				if(all_p[i].gen<3){
					v_need.push_back(make_pair(i,t_need+9));
					t_val=1+(deep-ds[my_planet][i])*(all_p[i].gen+1)-t_need;
					v_val.push_back(make_pair(t_val,t_id++));
				}
			}
		}
		if(all_p[my_planet].gen<3){
			t_need=10;
			t_val=deep*(all_p[my_planet].gen+1)-10;
			v_need.push_back(make_pair(my_planet,t_need));
			v_val.push_back(make_pair(t_val,t_id++));
		}
		sort(v_val.rbegin(),v_val.rend());
		int cnt=0,tgt;
		int sz=all_p[my_planet].cur;
		set<int> tgused;
		for(auto i:v_val){
			int id=i.second;
			cerr<<v_need[id].second<<"+"<<cnt<<"<"<<sz;
			if(v_need[id].second+cnt<sz){
				cerr<<" -> entered"<<endl;
				cnt+=v_need[id].second;
				tgt=v_need[id].first;
				if(tgt==my_planet){
					Command c(1,0,tgt,0,0);
					cerr<<"u1:chnged reserv on "<<tgt<<" from "<<reserved[tgt];
					reserved[tgt]+=10;
					cerr<<" to "<<reserved[tgt]<<endl;
					cm.push_back(c);
				}
				else{
					if(tgused.find(tgt)==tgused.end()){
						tgused.insert(tgt);
						Command c(0,my_planet,tgt,v_need[id].second,0);
						cerr<<"chnged reserv on "<<my_planet<<" from "<<reserved[my_planet];
						reserved[my_planet]+=v_need[id].second;
						cerr<<" to "<<reserved[my_planet]<<endl;
						cm.push_back(c);
						if(v_need[id].second>=all_p[tgt].cur+10){
							Command c(1,0,tgt,0,ds[my_planet][tgt]+1);
							cm.push_back(c);
							cerr<<"u2:chnged reserv on "<<tgt<<" from "<<reserved[tgt];
							reserved[tgt]=10;
							cerr<<" to "<<reserved[tgt]<<endl;
						}
					}
				}
			}
			else cerr<<endl;
		}
		state=1;
		//cerr<<v_val[0].first<<":"<<v_val[0].second<<"-"<<v_need[v_val[0].second].first<<"-"<<v_need[v_val[0].second].second<<endl;
	}
	void debut(){
		set<int> attacked;
		//analize previous command
		int bs[factoryCount];
		for(int i=0;i<factoryCount;i++){
			future[i][0]=all_p[i].cur;
			bs[i]=all_p[i].boss;
			if(bs[i]==0)future[i][0]+=KEY1;
		}
		for(int i=0;i<factoryCount;i++)
			for(int j=1;j<MAX_LEN;j++){
				future[i][j]=future[i][j-1]+bs[i]*all_p[i].base_gen;
				if(bs[i]==0){
					if(to_planet_troop[i][j]>0){
						future[i][j]-=to_planet_troop[i][j];
					}
					else{
						future[i][j]+=to_planet_troop[i][j];
					}
					if(future[i][j]<KEY1){
						future[i][j]-=KEY1;
						if(to_planet_troop[i][j]>0){
							future[i][j]*=-1;
							bs[i]=1;
						}
						else bs[i]=-1;
					}
				}
				else{
					future[i][j]+=to_planet_troop[i][j];
					if(bs[i]*future[i][j]<0)bs[i]*=-1;
				}
			}
		//analize enemy bombs
		for(auto i:bombs){
			if(i.boss==-1){
				
			}
		}
		//analize enemy troops
		for(auto i:en_t){
			int hsh=all_t[i].from*KEY1+all_t[i].to*KEY2+all_t[i].cur*KEY3+all_t[i].rem;
			int t=all_t[i].to;
			if(all_p[t].marker<10||all_p[t].marker==12){
				Plan p=plans[t];
				if(p.enemy_hsh.find(hsh)==p.enemy_hsh.end()){
					attacked.insert(t);
					p.enemy_hsh.insert(hsh);
				}
			}
		}
		for(auto i : attacked){
			cerr<<"attacked "<<i<<endl;
			int cur=all_p[i].cur;
			int cb =all_p[i].boss;
			int need=0,cbo;
			int change_step=-1;
			for(int st=0;st<MAX_LEN;st++){
				if(future[i][st]<0){
					change_step=st;
					cbo=1;
					need=0-future[i][st]+reserved[i];
					if(future[i][st-1]>KEY1-1){
						cbo=0;
						need=future[i][st-1]-KEY1+1;
						if(need>=2){
							need=0-future[i][st]+all_p[i].base_gen;
							change_step++;
						}
					}else cerr<<"needed: "<<need<<" step:"<<change_step<<" resesrv: "<<reserved[i]<<endl;
					break;
				}
			}
			if(change_step>=0){
				vector<Command> ccc;
				int variation;
				int src,cnt,mdst;
				if(cbo==0){
					if(need<2){
						for(auto j : my_p){
							if(ds[i][j]<change_step){
								cnt=all_p[j].cur-reserved[j];
								if(cnt>0){
									for(int k=0;k<MAX_LEN;k++){
										if(future[j][k]>0)
											cnt=min(cnt,future[j][k]);
									}
									cnt=min(cnt,need);
									Command c(0,j,i,cnt,change_step-ds[i][j]-1);
									ccc.push_back(c);
									need-=cnt;
									for(int k=0;k<MAX_LEN;k++){
										future[j][k]-=cnt;
										if(k>=change_step-1){
											if(future[i][k-1]>KEY1-1){
												future[i][k]=cnt-(future[i][k]-KEY1);
											}
											else{
												future[i][k]=future[i][k-1]+all_p[i].base_gen+to_planet_troop[i][k];
											}
										}
									}
								}
							}
							if(need==0) break;
						}
					}
					else{
						for(auto j : my_p){
							if(ds[i][j]<change_step){
								cnt=all_p[j].cur-reserved[j];
								if(cnt>0){
									for(int k=0;k<MAX_LEN;k++){
										if(future[j][k]>0)
											cnt=min(cnt,future[j][k]);
									}
									cnt=min(cnt,need);
									Command c(0,j,i,cnt,change_step-ds[i][j]);
									ccc.push_back(c);
									need-=cnt;
									for(int k=0;k<MAX_LEN;k++){
										future[j][k]-=cnt;
										if(k>=change_step-1){
											if(future[i][k-1]>KEY1-1){
												future[i][k]=cnt-(future[i][k]-KEY1);
											}
											else{
												future[i][k]=future[i][k-1]+all_p[i].base_gen+to_planet_troop[i][k];
											}
										}
									}
								}
							}
							if(need==0) break;
						}
					}
					if(need>0){
						for(auto j : my_p){
							if(all_p[j].cur>future[i][ds[i][j]]&&need>0){
								need=0;
								Command c(0,j,i,all_p[j].cur,0);
								ccc.push_back(c);
							}
						}
					}
				}
				else{
					for(auto j : my_p){
						if(i!=j&&ds[i][j]<change_step){
							cnt=all_p[j].cur-reserved[j];
							if(cnt>0){
								for(int k=0;k<MAX_LEN;k++){
									if(future[j][k]>0)
										cnt=min(cnt,future[j][k]);
								}
								cnt=min(cnt,need);
								Command c(0,j,i,cnt,change_step-ds[i][j]-1);
								ccc.push_back(c);
								need-=cnt;
								for(int k=0;k<MAX_LEN;k++){
									future[j][k]-=cnt;
									if(k>=change_step-1){
										if(future[i][k-1]>KEY1-1){
											future[i][k]=cnt-(future[i][k]-KEY1);
										}
										else{
											future[i][k]=future[i][k-1]+all_p[i].base_gen+to_planet_troop[i][k];
										}
									}
								}
							}
						}
						if(need==0) break;
					}
				}
				if(need>0)all_p[i].marker=10;
				else{
					for(auto c : ccc){
						cm.push_back(c);
						cerr<<"chnged reserv on "<<c.from<<" from "<<reserved[c.from];
						reserved[c.from]+=c.guard;
						cerr<<" to "<<reserved[c.from]<<endl;
					}
				}
			}
		}
		//create new command
		//finally steps
		for(auto i : plans){
			set<int> nhsh;
			for(auto j : i.enemy_hsh){
				if(j%100!=0) nhsh.insert(j-1);
			}
			i.enemy_hsh=nhsh;
		}
	}
	void defence(){
		
	}
	void get_final(){
		
	}
	void out(){
		int cnt=0;
		vector<int> passed;
		for(auto i=cm.begin();i!=cm.end();i++){
			//cerr<<"command "<<i->oper<<" counter "<<i->counter<<endl;
			if(i->counter==0){
				passed.push_back(cnt);
				switch(i->oper){
					case 0 :
						cout<<"MOVE "<<i->from<<" "<<i->target<<" "<<i->guard<<";";
						cerr<<"-chnged reserv on "<<i->from<<" from "<<reserved[i->from];
						reserved[i->from]-=i->guard;
						cerr<<" to "<<reserved[i->from]<<endl;
						break;
					case 1 :
						cerr<<"-chnged reserv on "<<i->target<<" from "<<reserved[i->target];
						reserved[i->target]-=10;
						cerr<<" to "<<reserved[i->target]<<endl;
						cout<<"INC "<<i->target<<";";
						break;
				}
			}
			else i->counter--;
			cnt++;
		}
		cout<<"WAIT"<<endl;
		for(auto i=passed.rbegin();i!=passed.rend();i++) cm.erase(cm.begin()+*i);
		turn++;
		for(int i=0;i<factoryCount;i++){
			if(reserved[i]>0)cerr<<"planet:"<<i<<" reserv:"<<reserved[i]<<endl;
		}
	}
	void calc(){
		switch(state){
			case 0 : 
				get_plan();
				break;
			case 1 : 
				debut();
				break;
			case 2 : 
				defence();
				break;
			case 3 : 
				get_final();
				break;
		}
		out();
	}
};
 
int main()
{
    int factoryCount; // the number of factories
    cin >> factoryCount; cin.ignore();
    int linkCount; // the number of links between factories
    cin >> linkCount; cin.ignore();
	Solution sl(factoryCount);
    for (int i = 0; i < linkCount; i++) {
        int factory1;
        int factory2;
        int distance;
        cin >> factory1 >> factory2 >> distance; cin.ignore();
		sl.add_dist(factory1,factory2,distance);
    }

    // game loop
    while (1) {
		sl.init();
        int entityCount; // the number of entities (e.g. factories and troops)
        
        cin >> entityCount; cin.ignore();
        int id=0;
        for (int i = 0; i < entityCount; i++) {
            int entityId;
            string entityType;
            int arg1;
            int arg2;
            int arg3;
            int arg4;
            int arg5;
            cin >> entityId >> entityType >> arg1 >> arg2 >> arg3 >> arg4 >> arg5; cin.ignore();
            //cerr<< entityId << entityType << arg1 << arg2 << arg3 << arg4 << arg5<<endl;
            if(entityType=="FACTORY"){
                sl.add_planet(arg1, arg2, arg3);
            } else {
                if(entityType=="BOMB")
					sl.add_bomb(arg1, arg2, arg3, arg4);
				else
					sl.add_troop(arg1, arg2, arg3, arg4, arg5);
            }
        }
		sl.calc();
    }
}
