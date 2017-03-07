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
        //cerr<<"X:"<<X<<" sp:"<<hSpeed<<" ang:"<<ang<<" sin:"<<sin(PI*ang/180)<<" d_x: "<< pw*100*sin(PI*ang/180)/2<<endl;
        int nx=(X+hSpeed+(int)(pw*100*(m_sin.get(ang)/MUL)/2));
        int vx=(hSpeed+(pw*100*(m_sin.get(ang)/MUL)));
        int ny=(Y+vSpeed+(int)((pw*100*(m_sin.getc(ang)/MUL)-gM)/2));
        int vy=vSpeed+(int)(pw*100*(m_sin.getc(ang)/MUL)-gM);
        //cerr<<"from x:"<<X<<" with ang "<<ang<<" and pow: "<<ipw<<" to x:"<<nx<<endl;
        //cerr<<"dX:"<<X+hSpeed<<" dvX:"<<(pw*sin(ang/PI)/2)<<endl;
        //cerr<<"dY:"<<Y+vSpeed<<" dvY:"<<(pw*cos(ang/PI)-gM)/2<<endl;
        //cerr<<"X:"<<nx<<" vX:"<<vx<<" Y:"<<ny<<" vY:"<<vy<<endl;
        rez.init(nx,ny,vx,vy,fuel-ipw,ang,ipw);
        return rez;
    }
};

class Graph{
private:
    const int MaxVx=2000;
    const int MaxVy=4000;
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
    
    int get_cost(Shuttle sh1, Shuttle sh2){
        if(sh2.getX()>=ml1+1000&&sh2.getX()<=ml2-1000&&sh2.getY()<=my&&sh2.Possible()) return WIN;
        if(abs(sh2.getAngle())>90)return -1;
        if(sh2.getX()<0||sh2.getX()>700000||sh2.getY()>300000) return -1;
        pt sf_start,sf_end,sh_start, sh_end;
        sh_start.x=sh1.getX();
        sh_start.y=sh1.getY();
        sh_end.x=sh2.getX();
        sh_end.y=sh2.getY();
        sf_start.x=sx[0];
        sf_start.y=sy[0];
        for(int it=1;it<surfaceN;it++){
            sf_end.x=sx[it];
            sf_end.y=sy[it];
            if(intersect(sh_start,sh_end,sf_start,sf_end))return -1;
            sf_start=sf_end;
        }
        int n_x=sh2.getX()-(ml2+ml1)/2;
        int n_y=sh2.getY()-my;
        //cerr<<"dx,dy="<<sh2.getX()<<"-"<<(ml2+ml1)/2<<"="<<n_x<<","<<n_y<<" cost:"<<(int)(5000000-sqrt(pow(n_x,2)*3+pow(n_y,2)))<<endl;
        return (5000000-sqrt(pow(n_x,2)*3+pow(n_y,2)));
    }
    
    void next_stage(Shuttle sh, int step,int d_a, int pw){
        Shuttle nxt=sh;
        //cerr<<"testing ang "<<d_a<<" pw "<<pw<<endl;
        nxt=nxt.next(d_a,pw,gMars,m_sin);
        
        for(int it=0;it<2;it++)
            nxt=nxt.next(0,pw,gMars,m_sin);
        //cerr<<"x changed from:"<<sh.getX()<<" to "<<nxt.getX()<<endl;
        int cost=get_cost(sh,nxt);
        if(cost<0) return;
        if(cost>=WIN){
            cerr<<"p_WIN cost:"<<cost<<" a:"<<c_a<<" pw:"<<c_p<<endl;
            max_cost=cost;
            rez_a=nomad.getAngle()-c_a;
            rez_p=c_p;
        }
        if(step<3){
            for(int n_a=-15;n_a<=15;n_a+=5){
                next_stage(nxt,step+1,n_a,0);
                next_stage(nxt,step+1,n_a,4);
            }
        }
        else{
            if(cost>max_cost){
                //cerr<<"["<<nxt.getX()<<","<<nxt.getY()<<"]"<<endl;
                //cerr<<"p cost:"<<cost<<" a:"<<c_a<<" pw:"<<c_p<<endl;
                max_cost=cost;
                rez_a=nomad.getAngle()-c_a;
                rez_p=c_p;
            }
            if(c_a==15){
                if(cost>m_c15){
                    //cerr<<"15/["<<nxt.getX()<<","<<nxt.getY()<<"]"<<endl;
                    m_c15=cost;
                    //cerr<<"15: "<<m_c15<<endl;
                }
            }
        }
        if(step==0)cerr<<"passed:"<<d_a<<endl;
    }
    
    void compare(int X,int Y,int hSpeed,int vSpeed,int fuel,int rotate,int power){
        //cerr<<test.getX()-X<<":"<<test.getY()-Y<<endl;
        if(abs(nomad.getX()-X+nomad.getY()-Y)>300)
            nomad.init(X,Y,hSpeed,vSpeed,fuel,rotate,power);
        max_cost=-1;
        m_c15=-1;
        int step=0;
        for(int d_a=-15;d_a<=15;d_a+=5){
            c_a=d_a;
            c_p=0;
            next_stage(nomad,step,d_a,0);
            c_p=4;
            next_stage(nomad,step,d_a,4);
        }
        
        cerr<<"it cost:"<<max_cost<<endl;
        //test=nomad.next(-15,1,gMars);
    }
    void first(int X,int Y,int hSpeed,int vSpeed,int fuel,int rotate,int power){
        nomad.init(X,Y,hSpeed,vSpeed,fuel,rotate,power);
        compare(X,Y,hSpeed,vSpeed,fuel,rotate,power);
        //test=nomad.next(-15,1,gMars);
        //cerr<<"passed!"<<endl;
    }
    
    pair<int,int> out(){
        cerr<<rez_a<<"-=-"<<rez_p<<endl;
        nomad=nomad.next(rez_a,rez_p,gMars,m_sin);
        return make_pair(rez_a,rez_p);
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
        pair<int,int> rez=sl.out();
        cout << rez.first<< " " << rez.second << endl;
    }
}
