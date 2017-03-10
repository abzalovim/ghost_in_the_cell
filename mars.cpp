#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>

#define PI 3.14159265
#define WIN -10000000
#define MUL 10000
#define LL long long

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
 
class Mt{
    int angles[36];
    int anglesc[36];
    public:
        void init(){
            for(int a=-90;a<=90;a+=5){
                angles[(90+a)/5]=sin(PI*a/180)*MUL;
                anglesc[(90+a)/5]=cos(PI*a/180)*MUL;
            }
        }
        int get(int a){
            //cerr<<"sin("<<a<<")="<<angles[(90+a)/5]<<endl;
            return angles[(90+a)/5];
        }
        int getc(int a){
            return anglesc[(90+a)/5];
        }
};

struct pt {
	int x, y;
};

inline int area (pt a, pt b, pt c) {
	return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}
 
inline bool intersect_1 (int a, int b, int c, int d) {
	if (a > b)  swap (a, b);
	if (c > d)  swap (c, d);
	return max(a,c) <= min(b,d);
}
 
bool intersect (pt a, pt b, pt c, pt d) {
	return intersect_1 (a.x, b.x, c.x, d.x)
		&& intersect_1 (a.y, b.y, c.y, d.y)
		&& area(a,b,c) * area(a,b,d) <= 0
		&& area(c,d,a) * area(c,d,b) <= 0;
}

class Shuttle{
private:
    LL X;
    LL Y;
    int hSpeed;
    int vSpeed;
    int fuel;
    int rotate;
    int power;
public:
    void init(LL p1,LL p2, int p3, int p4, int p5, int p6, int p7){
        X=p1;
        Y=p2;
        hSpeed=p3;
        vSpeed=p4;
        fuel=p5;
        rotate=p6;
        power=p7;
        //cerr<<"init Y:"<<Y<<endl;
    }
    
    int getAngle() {return rotate;}
    LL getX() {return X;}
    LL getY() {return Y;}
    int getVX() {return hSpeed;}
    int getVY() {return vSpeed;}
    bool Possible(){return abs(hSpeed)<=2000&&abs(vSpeed)<=4000;}
    
    Shuttle next(int n_ang, int ipw, double gM, Mt m_sin){
        Shuttle rez;
        double pw=(abs(ipw-power)>1 ? (ipw<power ? power-1 : power+1) : ipw);
        int d_ang=min(15,abs(rotate-n_ang));
        int ang=(rotate>n_ang ? rotate-d_ang : rotate+d_ang);
        LL nx=(X+hSpeed+(int)(((pw*100*(m_sin.get(-ang))/MUL)/2)));
        int vx=(hSpeed+((pw*100*m_sin.get(-ang))/MUL));
        //cerr<<"vx:"<<hSpeed<<"+(("<<pw<<"*100*"<<m_sin.get(ang)<<")/"<<MUL<<")) = "<<vx<<endl;
        LL ny=(Y+vSpeed+(int)(((pw*100*m_sin.getc(-ang))/MUL)-gM)/2);
        int vy=vSpeed+(int)(((pw*100*m_sin.getc(-ang))/MUL)-gM);
        
        rez.init(nx,ny,vx,vy,fuel-ipw,ang,ipw);
        return rez;
    }
};

class Leaf{
public:
    int parent_id;
    Shuttle n;
    int angle;
    int power;
};

class Graph{
private:
    const int MaxVx=1800;
    const int MaxVy=3800;
    const double gMars=371.1;
    map<int,vector<Shuttle>> steps;
    vector<int> sx,sy;
    int surfaceN;
    int ml1,ml2,my;
    int old_y=-10000000,old_x=0;
    int it;
    Shuttle nomad;
    Shuttle test;
    int rez_a,rez_p,c_a,c_p,max_cost;
    int m_c15;
    Mt m_sin;
    LL alt[7000];
    LL cc1,cc2,cc3;
public:
    void init(int sN){
        surfaceN=sN;
        sx.resize(sN);
        sy.resize(sN);
        it=0;
        m_sin.init();
    }
    void add_point(int landX, int landY){
        landX*=100;
        landY*=100;
        if(it>0){
            for(int xx=sx[it-1]/100;xx<landX/100;xx++){
                alt[xx]=sy[it-1]+(xx*100-sx[it-1])*(landY-sy[it-1])/(landX-sx[it-1])+500;
            }
        }
        sx[it]=landX;sy[it]=landY;
        it++;
        if(old_x>=0){
            if(landY==old_y){
                if(landX-old_x>99999){
                    ml1=old_x;
                    ml2=landX;
                    my=landY;
                    old_x=-1;
                    cerr<<"land is "<<ml1<<":"<<ml2<<","<<my<<endl;
                }
                else{
                    old_y=landY;
                    old_x=landX;
                }
            }
            else {
                old_y=landY;
                old_x=landX;
            }
        }
    }
    LL get_cost(Shuttle n){
		LL _x=n.getX();
		LL _y=n.getY();
		LL _vx=n.getVX();
		LL _vy=n.getVY();
		LL avx=abs(_vx);
		LL avy=abs(_vy);
		
		if(_x<=ml2-4500&&_x>=ml1+4500&&_y<=my&&avx<=1900&&avy<=3900) return WIN;
		LL _mid_x=(ml1+ml2)/2;
		_x+=_vx*15;
		LL cost1=sqrt(((_x-_mid_x)*(_x-_mid_x)+(_y-my)*(_y-my)/5))*5;
		if((_x<ml1&&_vx<=0)||(_x>ml2&&_vx>=0)) avx+=100000;
		LL cost2=(avx>2000 ? 1.5*avx : avx);
		LL cost3=(avy>4000 ? (avy>4400 ? 2500*avy : 2*avy) : avy);
		cc1=cost1;
		cc2=cost2;
		cc3=cost3;
		return cost1+cost2+cost3;
	}
	
    void compare(LL X,LL Y,int hSpeed,int vSpeed,int fuel,int rotate,int power){
        Shuttle n;
        if(X<=ml2-4500&&X>=ml1+4500&&Y<=my+15000&&abs(hSpeed)<=1900&&abs(vSpeed)<=3900){
            rez_p=(abs(vSpeed)>3000 ? 4 : 3);
            rez_a=0;
            return;
        }
        LL cost=-10*WIN;
        rez_p=0;
        rez_a=-90;
        for(int p=max(0,power-1);p<=min(4,power+1);p++){
			for(int a=max(-90,rotate-15);a<=min(90,rotate+15);a+=5){
			    n.init(X,Y,hSpeed,vSpeed,fuel,rotate,power);
				cerr<<"##"<<a<<":"<<p<<"=("<<n.getX()<<":"<<n.getY()<<") ";
			    n=n.next(a, p, gMars, m_sin);
			    cerr<<" st:1 "<<n.getX()<<":"<<n.getY()<<" V "<<n.getVX()<<":"<<n.getVY()<<endl;
				n=n.next(a, p, gMars, m_sin);
			    cerr<<" st:2 "<<n.getX()<<":"<<n.getY()<<" V "<<n.getVX()<<":"<<n.getVY()<<endl;
				n=n.next(a, p, gMars, m_sin);
			    cerr<<" st:3 "<<n.getX()<<":"<<n.getY()<<" V "<<n.getVX()<<":"<<n.getVY()<<endl;
				LL n_cost=get_cost(n);
				cerr<<cc1<<"+"<<cc2<<"+"<<cc3<<"="<<n_cost<<endl;
				if(n_cost<cost){
					cost = n_cost;
					rez_a= a;
					rez_p= p;
					cerr<<a<<":"<<p<<" => "<<cc1<<"+"<<cc2<<"+"<<cc3<<"="<<n_cost<<endl;
				}
			}
		}
    }
    
    void found_way(vector<Leaf>* g, int st_id){
        Leaf l = (*g)[st_id];
        int power=l.power;
        int rotate=l.angle;
        for(int p=max(0,power-1);p<=min(4,power+1);p++)
            for(int a=max(-90,rotate-15);a<=min(90,rotate+15);a+=5){
            }
    }
    
    void first(LL X,LL Y,int hSpeed,int vSpeed,int fuel,int rotate,int power){
        vector<Leaf> graph;
        nomad.init(X,Y,hSpeed,vSpeed,fuel,rotate,power);
        int id=0;
        Leaf l;
        l.n=nomad;
        l.parent_id=id;
        l.angle = rotate;
        l.power = power;
        graph.push_back(l);
        found_way(&graph,0);
        compare(X,Y,hSpeed,vSpeed,fuel,rotate,power);
    }
    
    void out(){
        nomad=nomad.next(rez_a,rez_p,gMars,m_sin);
        cout<<rez_a<<" "<<rez_p<<endl;
    }
    
};
 
int main()
{
    int surfaceN; // the number of points used to draw the surface of Mars.
    cin >> surfaceN; cin.ignore();
    Graph sl;
    sl.init(surfaceN);
    
    for (int i = 0; i < surfaceN; i++) {
        int landX; // X coordinate of a surface point. (0 to 6999)
        int landY; // Y coordinate of a surface point. By linking all the points together in a sequential fashion, you form the surface of Mars.
        cin >> landX >> landY; cin.ignore();
        sl.add_point(landX,landY);
    }
    bool fl_init=false;
    // game loop
    while (1) {
        int X;
        int Y;
        int hSpeed; // the horizontal speed (in m/s), can be negative.
        int vSpeed; // the vertical speed (in m/s), can be negative.
        int fuel; // the quantity of remaining fuel in liters.
        int rotate; // the rotation angle in degrees (-90 to 90).
        int power; // the thrust power (0 to 4).
        cin >> X >> Y >> hSpeed >> vSpeed >> fuel >> rotate >> power; cin.ignore();
        if(!fl_init){
            sl.first(X*100,Y*100,hSpeed*100,vSpeed*100,fuel,rotate,power);
            //sl.compare(X*100,Y*100,hSpeed*100,vSpeed*100,fuel,rotate,power);
            fl_init=true;
        }
        else{
            sl.compare(X*100,Y*100,hSpeed*100,vSpeed*100,fuel,rotate,power);
        }

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;


        // rotate power. rotate is the desired rotation angle. power is the desired thrust power.
        sl.out();
    }
}
