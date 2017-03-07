#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>

#define PI 3.14159265
#define WIN 10000000
#define MUL 10000

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
 
class Mt{
    int angles[34];
    int anglesc[34];
    public:
        void init(){
            for(int a=-85;a<=85;a+=5){
                angles[(85+a)/5]=sin(PI*a/180)*MUL;
                anglesc[(85+a)/5]=cos(PI*a/180)*MUL;
            }
        }
        int get(int a){
            return angles[(85+a)/5];
        }
        int getc(int a){
            return anglesc[(85+a)/5];
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
    int X;
    int Y;
    int hSpeed;
    int vSpeed;
    int fuel;
    int rotate;
    int power;
public:
    void init(int p1,int p2, int p3, int p4, int p5, int p6, int p7){
        X=p1;
        Y=p2;
        hSpeed=p3;
        vSpeed=p4;
        fuel=p5;
        rotate=p6;
        power=p7;
    }
    
    int getAngle() {return rotate;}
    int getX() {return X;}
    int getY() {return Y;}
    int getVX() {return hSpeed;}
    int getVY() {return vSpeed;}
    bool Possible(){return abs(hSpeed)<=20&&abs(vSpeed)<=40;}
    
    Shuttle next(int d_ang, int ipw, double gM, Mt m_sin){
        Shuttle rez;
        double pw=(abs(ipw-power)>1 ? (ipw<power ? power-1 : power+1) : ipw);
        int ang=rotate-d_ang;
        int nx=(X+hSpeed+(int)(pw*100*(m_sin.get(ang)/MUL)/2));
        int vx=(hSpeed+(pw*100*(m_sin.get(ang)/MUL)));
        int ny=(Y+vSpeed+(int)((pw*100*(m_sin.getc(ang)/MUL)-gM)/2));
        int vy=vSpeed+(int)(pw*100*(m_sin.getc(ang)/MUL)-gM);
        
        rez.init(nx,ny,vx,vy,fuel-ipw,ang,ipw);
        return rez;
    }
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
    
    void compare(int X,int Y,int hSpeed,int vSpeed,int fuel,int rotate,int power){
        //cerr<<test.getX()-X<<":"<<test.getY()-Y<<endl;
        if(abs(nomad.getX()-X+nomad.getY()-Y)>300)
            nomad.init(X,Y,hSpeed,vSpeed,fuel,rotate,power);

        int t, ny, cx, cy;
        
        if(abs(hSpeed)>MaxVx){
			//Надо притормозить
			rez_p=4;
			rez_a=30;
			if(hSpeed<0){
				rez_a=-30;
			}
			return;
		}
		rez_p =(abs(vSpeed)>MaxVy ? 4 : 3);
		rez_a=0;
    }
    void first(int X,int Y,int hSpeed,int vSpeed,int fuel,int rotate,int power){
        nomad.init(X,Y,hSpeed,vSpeed,fuel,rotate,power);
        compare(X,Y,hSpeed,vSpeed,fuel,rotate,power);
        //test=nomad.next(-15,1,gMars);
        //cerr<<"passed!"<<endl;
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
