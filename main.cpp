#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#define MAX_PLANET 15

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
    void init(int b, int f, int t, int c, int r){
        boss=b;
        from=f;
        to  =t;
        cur =c;
        rem =r;
    }
};

class User{
public:
	int bombs;
	vector<Planet> pl;
	vector<Troop> tr;
	User(){
		bombs=2;
	}
	void init(){
		pl.clear();
		tr.clear();
	}
	void add_pl(int c, int g){
		
	}
};

class Solution{
private:
	int state; //0-init, 1-debut, 2-mid, 3-end
	int plan;
	int pl_id, m_cnt, e_cnt, n_cnt;
	int factoryCount;
	int ds[MAX_PLANET][MAX_PLANET];
	Planet all_p[MAX_PLANET];
	vector<int> my_p,en_p,ne_p;
public:
	Solution(int fC){
		state=0;
		plan =0;
		factoryCount = fC;
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
		if(b==1)my_p.push_back(pl_id);
		else if(b==-1)en_p.push_back(pl_id);
		else ne_p.push_back(pl_id);
		pl_id++;
	}
	void add_troop(int b, int f, int t, int c, int r){
		
	}
	void add_bomb(int b, int f, int t, int r){
		
	}
	void init(){
		pl_id=0;
		m_cnt=0;
		e_cnt=0;
		n_cnt=0;
	}
	//------------------------------------------logic------------------------------------------
	void create_markers(){
		for(auto i : my_p){
			for(int j=0;j<factoryCount;j++){
				int d1,d2;
				d1=ds[*i][j];
				d2=9999;
				for(auto k : en_p)
					d2=min(d2,ds[*k][j]);
				if(d1<d2-3)all_p[j].marker=0;// my zone
				else if(d2<d1-3)all_p[j].marker=10; //enemy
				else if(d1<d2)
					if((all_p[j].base_gen>1)&&(all_p[j].cur>6))all_p[j].marker=1;//my opt
					else all_p[j].marker=2;//my nw
				else if(d2>d1)
					if((all_p[j].base_gen>1)&&(all_p[j].cur>6))all_p[j].marker=11;//en opt
					else all_p[j].marker=12;//en nw
				else all_p[j].marker=100;
			}
		}
	}
	void get_plan(){
		create_markers();
		if()
	}
	void debut(){
		
	}
	void defence(){
		
	}
	void get_final(){
		
	}
	void out(){
		cout<<"WAIT"<<endl;
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